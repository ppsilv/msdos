#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>

#define BIOS_SEG 0xF000
#define BIOS_START 0xF0000UL
#define BIOS_END 0xFFFFFUL
#define MAX_ENTRIES 256

FILE *asm_file;

typedef struct {
    const char *name;
    int length;
    const char *format;
} Instruction;

typedef struct {
    unsigned short int_num;
    unsigned short seg;
    unsigned short off;
} BiosEntry;

BiosEntry entries[MAX_ENTRIES];
int total_entries = 0;

/* Tabela de instruções com formatos de operandos */
const Instruction inst_table[256] = {
    [0x00] = {"add", 2, "Eb,Gb"},    [0x01] = {"add", 2, "Ev,Gv"},
    [0x02] = {"add", 2, "Gb,Eb"},    [0x03] = {"add", 2, "Gv,Ev"},
    [0x04] = {"add", 2, "AL,Ib"},    [0x05] = {"add", 3, "AX,Iv"},
    [0x08] = {"or",  2, "Eb,Gb"},    [0x09] = {"or",  2, "Ev,Gv"},
    [0x0A] = {"or",  2, "Gb,Eb"},    [0x0B] = {"or",  2, "Gv,Ev"},
    [0x10] = {"adc", 2, "Eb,Gb"},    [0x11] = {"adc", 2, "Ev,Gv"},
    [0x12] = {"adc", 2, "Gb,Eb"},    [0x13] = {"adc", 2, "Gv,Ev"},
    [0x20] = {"and", 2, "Eb,Gb"},    [0x21] = {"and", 2, "Ev,Gv"},
    [0x22] = {"and", 2, "Gb,Eb"},    [0x23] = {"and", 2, "Gv,Ev"},
    [0x24] = {"and", 2, "AL,Ib"},    [0x25] = {"and", 3, "AX,Iv"},
    [0x30] = {"xor", 2, "Eb,Gb"},    [0x31] = {"xor", 2, "Ev,Gv"},
    [0x32] = {"xor", 2, "Gb,Eb"},    [0x33] = {"xor", 2, "Gv,Ev"},
    [0x38] = {"cmp", 2, "Eb,Gb"},    [0x39] = {"cmp", 2, "Ev,Gv"},
    [0x3A] = {"cmp", 2, "Gb,Eb"},    [0x3B] = {"cmp", 2, "Gv,Ev"},
    [0x50] = {"push",1, "AX"},       [0x51] = {"push",1, "CX"},
    [0x68] = {"push",3, "Iv"},       [0x6A] = {"push",2, "Ib"},
    [0x70] = {"jo",  2, "Jb"},       [0x71] = {"jno", 2, "Jb"},
    [0x72] = {"jb",  2, "Jb"},       [0x73] = {"jnb", 2, "Jb"},
    [0x74] = {"jz",  2, "Jb"},       [0x75] = {"jnz", 2, "Jb"},
    [0x88] = {"mov", 2, "Eb,Gb"},    [0x89] = {"mov", 2, "Ev,Gv"},
    [0x8A] = {"mov", 2, "Gb,Eb"},    [0x8B] = {"mov", 2, "Gv,Ev"},
    [0x8C] = {"mov", 2, "Ev,Sw"},    [0x8E] = {"mov", 2, "Sw,Ev"},
    [0x9A] = {"call",5, "Ap"},       [0xA0] = {"mov", 3, "AL,Ov"},
    [0xA1] = {"mov", 3, "AX,Ov"},    [0xA2] = {"mov", 3, "Ov,AL"},
    [0xA3] = {"mov", 3, "Ov,AX"},    [0xB0] = {"mov", 2, "AL,Ib"},
    [0xB8] = {"mov", 3, "AX,Iv"},    [0xC3] = {"ret", 1, ""},
    [0xC9] = {"leave",1, ""},        [0xCD] = {"int", 2, "Ib"},
    [0xCF] = {"iret",1, ""},         [0xE8] = {"call",3, "Jv"},
    [0xE9] = {"jmp", 3, "Jv"},       [0xEA] = {"jmp", 5, "Ap"},
    [0xEB] = {"jmp", 2, "Jb"},       [0xFA] = {"cli", 1, ""},
    [0xFB] = {"sti", 1, ""}
};

const char *reg8[]  = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char *reg16[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *seg_reg[] = {"es", "cs", "ss", "ds", "fs", "gs"};

void decode_operand(unsigned char far *code, int *index, const char type, unsigned short seg) {
    switch(type) {
        case 'b': printf("byte "); break;
        case 'v': printf("word "); break;
    }

    switch(type) {
        case 'A': // Ap - Ponteiro absoluto
            printf("%04X:%04X",
                  *((unsigned short far*)(code + *index + 3)),
                  *((unsigned short far*)(code + *index + 1)));
            *index += 4;
            break;

        case 'I': // Immediate
            if(type == 'b') {
                printf("0x%02X", code[*index]);
                *index += 1;
            } else {
                printf("0x%04X", *((unsigned short far*)(code + *index)));
                *index += 2;
            }
            break;

        case 'J': // Jump offset
        {
            short offset = (type == 'b') ? (signed char)code[*index]
                                        : *((short far*)(code + *index));
            unsigned long target = ((unsigned long)seg << 4) + *index + offset +
                                 ((type == 'b') ? 2 : 3);
            printf("0x%05lX", target);
            *index += (type == 'b') ? 1 : 2;
            break;
        }

        case 'O': // Offset
            printf("[0x%04X]", *((unsigned short far*)(code + *index)));
            *index += 2;
            break;

        case 'E': // ModR/M
        {
            unsigned char modrm = code[*index];
            int mod = modrm >> 6;
            int reg = (modrm >> 3) & 7;
            int rm = modrm & 7;

            *index += 1;

            const char **regs = (type == 'b') ? reg8 : reg16;
            printf("%s", regs[reg]);

            if(mod == 3) {
                printf(",%s", regs[rm]);
            } else {
                printf(",[%s]", (rm == 6) ? "bp" : reg16[rm]);
                if(mod == 1) {
                    printf("+0x%02X", code[*index]);
                    *index += 1;
                } else if(mod == 2) {
                    printf("+0x%04X", *((unsigned short far*)(code + *index)));
                    *index += 2;
                }
            }
            break;
        }
    }
}

void disassemble(unsigned short seg, unsigned short off) {
    unsigned char far *code = MK_FP(seg, off);
    int index = 0, inst_len;
    unsigned char opcode;

    fprintf(asm_file, "\n; === BIOS Entry at %04X:%04X ===\n", seg, off);

    while(index < 128) { // Limite por entrada
        opcode = code[index];
        const Instruction *inst = &inst_table[opcode];

        if(inst->name == NULL) {
            fprintf(asm_file, "db 0x%02X\n", opcode);
            index++;
            continue;
        }

        // Imprime endereço
        fprintf(asm_file, "%04X:%04X  ", seg, off + index);

        // Imprime bytes
        for(int i = 0; i < inst->length && i < 6; i++)
            fprintf(asm_file, "%02X ", code[index + i]);
        fprintf(asm_file, "%-*s ", (6 - inst->length) * 3, "");

        // Imprime instrução
        fprintf(asm_file, "%-6s", inst->name);

        // Decodifica operandos
        const char *fmt = inst->format;
        while(*fmt) {
            if(*fmt == ',') {
                fmt++;
                fprintf(asm_file, ", ");
                continue;
            }

            decode_operand(code, &index, *fmt, seg);
            fmt++;
        }

        fprintf(asm_file, "\n");
        index += inst->length;

        if(opcode == 0xC3 || opcode == 0xCB || opcode == 0xCF) // RET/RETF/IRET
            break;
    }
}
void write_asm_header(void)
{
    fprintf(asm_file, "; BIOS Disassembly\n");
    fprintf(asm_file, "; Generated by PCAT Disassembler\n\n");
    fprintf(asm_file, "CPU 386\n");
    fprintf(asm_file, "BITS 16\n\n");
    fprintf(asm_file, "section .text\n");
    fprintf(asm_file, "org 0x0000\n");
    fprintf(asm_file, "segment .text\n");
}

void print_header(void)
{
    printf("PCAT BIOS Disassembler Pro v2.0\n");
    printf("Target CPU: Pentium S 200MHz\n");
    printf("ROM Area: %04X:%04X-%04X\n\n",
           BIOS_SEG, 0, BIOS_SEG, BIOS_END - (BIOS_SEG << 4));
}

void scan_ivt(void)
{
    int i;
    printf("Scanning IVT for BIOS entries...\n");

    for(i = 0; i < 256; i++) {
        unsigned short seg = peek(0, i*4 + 2);
        unsigned short off = peek(0, i*4);
        unsigned long addr = ((unsigned long)seg << 4) + off;

        if(is_valid_bios_address(addr)) {
            entries[total_entries].int_num = i;
            entries[total_entries].seg = seg;
            entries[total_entries].off = off;
            total_entries++;
        }
    }
}

void full_disassembly(void)
{
    int i;
    printf("\nStarting Full Disassembly...\n");

    for(i = 0; i < total_entries; i++) {
        disassemble(entries[i].seg, entries[i].off);
    }
}

void verify_signatures(void)
{
    unsigned char far *reset_vec = MK_FP(0xF000, 0xFFF0);

    printf("\nVerifying BIOS signatures...\n");
    if(memcmp(reset_vec, "\xEA\x5B\xE0\x00\xF0", 5) == 0) {
        printf("Valid PCAT BIOS signature found at F000:FFF0\n");
    }
}

int main(void)
{
    int i;
    char filename[] = "BIOS_DIS.ASM";

    clrscr();
    asm_file = fopen(filename, "w");
    if(!asm_file) {
        printf("Erro ao criar arquivo %s\n", filename);
        return 1;
    }

    write_asm_header();
    print_header();
    verify_signatures();
    scan_ivt();
    full_disassembly();

    fclose(asm_file);
    printf("\nDisassembly salvo em %s\n", filename);
    printf("Tamanho total: %lu bytes\n", ftell(asm_file));
    printf("Pressione qualquer tecla...");
    getch();
    return 0;
}
