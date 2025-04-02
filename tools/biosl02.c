#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>

#define BIOS_SEG 0xF000
#define BIOS_START 0xF0000UL
#define BIOS_END 0xFFFFFUL
#define MAX_ENTRIES 256

typedef struct {
    const char *name;
    int length;
    const char *format;
} Instruction;

/* Tabela de instruções C89 compatível */
const Instruction inst_table[256] = {
    /* 0x00 */ {"add", 2, "Eb,Gb"}, {"add", 2, "Ev,Gv"}, {"add", 2, "Gb,Eb"}, {"add", 2, "Gv,Ev"},
    /* 0x04 */ {"add", 2, "AL,Ib"}, {"add", 3, "AX,Iv"}, {"", 0, ""}, {"", 0, ""},
    /* 0x08 */ {"or",  2, "Eb,Gb"}, {"or",  2, "Ev,Gv"}, {"or",  2, "Gb,Eb"}, {"or",  2, "Gv,Ev"},
    /* 0x0C */ {"", 0, ""}, {"", 0, ""}, {"", 0, ""}, {"", 0, ""},
    /* 0x10 */ {"adc", 2, "Eb,Gb"}, {"adc", 2, "Ev,Gv"}, {"adc", 2, "Gb,Eb"}, {"adc", 2, "Gv,Ev"},
    /* 0x14 */ {"", 0, ""}, {"", 0, ""}, {"", 0, ""}, {"", 0, ""},
    /* 0x18 */ {"sbb", 2, "Eb,Gb"}, {"sbb", 2, "Ev,Gv"}, {"sbb", 2, "Gb,Eb"}, {"sbb", 2, "Gv,Ev"},
    /* 0x1C */ {"", 0, ""}, {"", 0, ""}, {"", 0, ""}, {"", 0, ""},
    /* 0x20 */ {"and", 2, "Eb,Gb"}, {"and", 2, "Ev,Gv"}, {"and", 2, "Gb,Eb"}, {"and", 2, "Gv,Ev"},
    /* 0x24 */ {"and", 2, "AL,Ib"}, {"and", 3, "AX,Iv"}, {"", 0, ""}, {"", 0, ""},
    /* 0x28 */ {"sub", 2, "Eb,Gb"}, {"sub", 2, "Ev,Gv"}, {"sub", 2, "Gb,Eb"}, {"sub", 2, "Gv,Ev"},
    /* 0x2C */ {"", 0, ""}, {"", 0, ""}, {"", 0, ""}, {"", 0, ""},
    /* 0x30 */ {"xor", 2, "Eb,Gb"}, {"xor", 2, "Ev,Gv"}, {"xor", 2, "Gb,Eb"}, {"xor", 2, "Gv,Ev"},
    /* 0x34 */ {"", 0, ""}, {"", 0, ""}, {"", 0, ""}, {"", 0, ""},
    /* 0x38 */ {"cmp", 2, "Eb,Gb"}, {"cmp", 2, "Ev,Gv"}, {"cmp", 2, "Gb,Eb"}, {"cmp", 2, "Gv,Ev"},
    /* 0x3C */ {"", 0, ""}, {"", 0, ""}, {"", 0, ""}, {"", 0, ""},
    /* 0x40 */ {"inc", 1, "AX"}, {"inc", 1, "CX"}, {"inc", 1, "DX"}, {"inc", 1, "BX"},
    /* 0x44 */ {"inc", 1, "SP"}, {"inc", 1, "BP"}, {"inc", 1, "SI"}, {"inc", 1, "DI"},
    /* 0x48 */ {"dec", 1, "AX"}, {"dec", 1, "CX"}, {"dec", 1, "DX"}, {"dec", 1, "BX"},
    /* 0x4C */ {"dec", 1, "SP"}, {"dec", 1, "BP"}, {"dec", 1, "SI"}, {"dec", 1, "DI"},
    /* 0x50 */ {"push",1, "AX"}, {"push",1, "CX"}, {"push",1, "DX"}, {"push",1, "BX"},
    /* 0x54 */ {"push",1, "SP"}, {"push",1, "BP"}, {"push",1, "SI"}, {"push",1, "DI"},
    /* 0x58 */ {"pop", 1, "AX"}, {"pop", 1, "CX"}, {"pop", 1, "DX"}, {"pop", 1, "BX"},
    /* 0x5C */ {"pop", 1, "SP"}, {"pop", 1, "BP"}, {"pop", 1, "SI"}, {"pop", 1, "DI"},
    /* 0x60 */ {"pusha",1, ""}, {"popa",1, ""}, {"", 0, ""}, {"", 0, ""},
    /* 0x64 */ {"", 0, ""}, {"", 0, ""}, {"", 0, ""}, {"", 0, ""},
    /* 0x68 */ {"push",3, "Iv"}, {"imul",3, "Gv,Ev,Iv"}, {"push",2, "Ib"}, {"imul",3, "Gv,Ev,Ib"},
    /* 0x6C */ {"insb",1, ""}, {"insw",1, ""}, {"outsb",1, ""}, {"outsw",1, ""},
    /* 0x70 */ {"jo",  2, "Jb"}, {"jno", 2, "Jb"}, {"jb",  2, "Jb"}, {"jnb", 2, "Jb"},
    /* 0x74 */ {"jz",  2, "Jb"}, {"jnz", 2, "Jb"}, {"jbe", 2, "Jb"}, {"ja",  2, "Jb"},
    /* 0x78 */ {"js",  2, "Jb"}, {"jns", 2, "Jb"}, {"jp",  2, "Jb"}, {"jnp", 2, "Jb"},
    /* 0x7C */ {"jl",  2, "Jb"}, {"jge", 2, "Jb"}, {"jle", 2, "Jb"}, {"jg",  2, "Jb"},
    /* 0x80 */ {"grp1",2, "Eb,Ib"}, {"grp1",2, "Ev,Iv"}, {"grp1",2, "Eb,Ib"}, {"grp1",2, "Ev,Ib"},
    /* 0x84 */ {"test",2, "Eb,Gb"}, {"test",2, "Ev,Gv"}, {"xchg",2, "Eb,Gb"}, {"xchg",2, "Ev,Gv"},
    /* 0x88 */ {"mov", 2, "Eb,Gb"}, {"mov", 2, "Ev,Gv"}, {"mov", 2, "Gb,Eb"}, {"mov", 2, "Gv,Ev"},
    /* 0x8C */ {"mov", 2, "Ev,Sw"}, {"lea", 2, "Gv,M"}, {"mov", 2, "Sw,Ev"}, {"pop", 2, "Ev"},
    /* 0x90 */ {"nop", 1, ""}, {"xchg",1, "AX,CX"}, {"xchg",1, "AX,DX"}, {"xchg",1, "AX,BX"},
    /* 0x94 */ {"xchg",1, "AX,SP"}, {"xchg",1, "AX,BP"}, {"xchg",1, "AX,SI"}, {"xchg",1, "AX,DI"},
    /* 0x98 */ {"cbw", 1, ""}, {"cwd", 1, ""}, {"call",5, "Ap"}, {"fwait",1, ""},
    /* 0x9C */ {"pushf",1, ""}, {"popf", 1, ""}, {"sahf",1, ""}, {"lahf",1, ""},
    /* 0xA0 */ {"mov", 3, "AL,Ov"}, {"mov", 3, "AX,Ov"}, {"mov", 3, "Ov,AL"}, {"mov", 3, "Ov,AX"},
    /* 0xA4 */ {"movsb",1, ""}, {"movsw",1, ""}, {"cmpsb",1, ""}, {"cmpsw",1, ""},
    /* 0xA8 */ {"test",2, "AL,Ib"}, {"test",3, "AX,Iv"}, {"stosb",1, ""}, {"stosw",1, ""},
    /* 0xAC */ {"lodsb",1, ""}, {"lodsw",1, ""}, {"scasb",1, ""}, {"scasw",1, ""},
    /* 0xB0 */ {"mov", 2, "AL,Ib"}, {"mov", 2, "CL,Ib"}, {"mov", 2, "DL,Ib"}, {"mov", 2, "BL,Ib"},
    /* 0xB4 */ {"mov", 2, "AH,Ib"}, {"mov", 2, "CH,Ib"}, {"mov", 2, "DH,Ib"}, {"mov", 2, "BH,Ib"},
    /* 0xB8 */ {"mov", 3, "AX,Iv"}, {"mov", 3, "CX,Iv"}, {"mov", 3, "DX,Iv"}, {"mov", 3, "BX,Iv"},
    /* 0xBC */ {"mov", 3, "SP,Iv"}, {"mov", 3, "BP,Iv"}, {"mov", 3, "SI,Iv"}, {"mov", 3, "DI,Iv"},
    /* 0xC0 */ {"grp2",2, "Eb,Ib"}, {"grp2",2, "Ev,Ib"}, {"ret", 2, "Iw"}, {"ret", 1, ""},
    /* 0xC4 */ {"les", 2, "Gv,Mp"}, {"lds", 2, "Gv,Mp"}, {"mov", 2, "Ev,Iv"}, {"mov", 2, "Ev,Iv"},
    /* 0xC8 */ {"enter",3, "Iw,Ib"}, {"leave",1, ""}, {"retf",2, "Iw"}, {"retf",1, ""},
    /* 0xCC */ {"int3",1, ""}, {"int", 2, "Ib"}, {"into",1, ""}, {"iret",1, ""},
    /* 0xD0 */ {"grp2",2, "Eb,1"}, {"grp2",2, "Ev,1"}, {"grp2",2, "Eb,CL"}, {"grp2",2, "Ev,CL"},
    /* 0xD4 */ {"aam", 2, "Ib"}, {"aad", 2, "Ib"}, {"salc",1, ""}, {"xlat",1, ""},
    /* 0xD8 */ {"esc", 1, ""}, {"esc", 1, ""}, {"esc", 1, ""}, {"esc", 1, ""},
    /* 0xDC */ {"esc", 1, ""}, {"esc", 1, ""}, {"esc", 1, ""}, {"esc", 1, ""},
    /* 0xE0 */ {"loopne",2, "Jb"}, {"loope",2, "Jb"}, {"loop",2, "Jb"}, {"jcxz",2, "Jb"},
    /* 0xE4 */ {"in",  2, "AL,Ib"}, {"in",  2, "AX,Ib"}, {"out", 2, "Ib,AL"}, {"out", 2, "Ib,AX"},
    /* 0xE8 */ {"call",3, "Jv"}, {"jmp", 3, "Jv"}, {"jmp", 5, "Ap"}, {"jmp", 2, "Jb"},
    /* 0xEC */ {"in",  1, "AL,DX"}, {"in",  1, "AX,DX"}, {"out", 1, "DX,AL"}, {"out", 1, "DX,AX"},
    /* 0xF0 */ {"lock",1, ""}, {"", 0, ""}, {"repne",1, ""}, {"rep", 1, ""},
    /* 0xF4 */ {"hlt", 1, ""}, {"cmc", 1, ""}, {"grp3a",1, ""}, {"grp3b",1, ""},
    /* 0xF8 */ {"clc", 1, ""}, {"stc", 1, ""}, {"cli", 1, ""}, {"sti", 1, ""},
    /* 0xFC */ {"cld", 1, ""}, {"std", 1, ""}, {"grp4",1, ""}, {"grp5",1, ""}
};

/* ... (demais funções mantidas conforme versão anterior) ... */
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
