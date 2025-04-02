#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>

#define BIOS_SEG 0xF000
#define BIOS_START 0xF0000UL
#define BIOS_END 0xFFFFFUL
#define MAX_ENTRIES 256

typedef struct {
    char *name;
    int length;
    char *format;
} Instruction;

typedef struct {
    unsigned short int_num;
    unsigned short seg;
    unsigned short off;
} BiosEntry;

FILE *asm_file;
BiosEntry entries[MAX_ENTRIES];
int total_entries = 0;

/* Tabelas de registradores */
char *reg8[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
char *reg16[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
char *seg_reg[] = {"es", "cs", "ss", "ds"};

/* Tabela de instruções preenchida sequencialmente */
Instruction inst_table[256] = {
    {"add",2,"Eb,Gb"}, {"add",2,"Ev,Gv"}, {"add",2,"Gb,Eb"}, {"add",2,"Gv,Ev"},
    {"add",2,"AL,Ib"}, {"add",3,"AX,Iv"}, {"",0,""}, {"",0,""},
    {"or",2,"Eb,Gb"},  {"or",2,"Ev,Gv"},  {"or",2,"Gb,Eb"},  {"or",2,"Gv,Ev"},
    /* ... (preencher todas as 256 entradas) ... */
    {"mov",3,"AX,Iv"}, {"",0,""},         {"int",2,"Ib"},    {"iret",1,""}
};

void write_asm_header(void)
{
    fprintf(asm_file, "; BIOS Disassembly\n");
    fprintf(asm_file, "cpu 8086\n");
    fprintf(asm_file, "bits 16\n\n");
}

int is_valid_bios_address(unsigned long addr)
{
    return (addr >= BIOS_START && addr <= BIOS_END);
}

void decode_operand(unsigned char far *code, int *index, char type, unsigned short seg)
{
    unsigned short val16;
    unsigned char val8;
    char **regs;
    int mod, reg, rm;

    if(type == 'b' || type == 'v') {
        regs = (type == 'b') ? reg8 : reg16;
    }

    switch(type) {
        case 'A': /* Ponteiro absoluto far */
            val16 = *((unsigned short far*)(code + *index + 1));
            *index += 2;
            fprintf(asm_file, "0x%04X:0x%04X", *((unsigned short far*)(code + *index + 1)), val16);
            *index += 2;
            break;

        case 'I': /* Immediate */
            if(type == 'b') {
                val8 = code[(*index)++];
                fprintf(asm_file, "0x%02X", val8);
            } else {
                val16 = *((unsigned short far*)(code + *index));
                *index += 2;
                fprintf(asm_file, "0x%04X", val16);
            }
            break;

        case 'J': /* Jump offset */
            /* ... (implementação similar) ... */
            break;

        default:
            break;
    }
}

void disassemble(unsigned short seg, unsigned short off)
{
    unsigned char far *code = MK_FP(seg, off);
    int index = 0;
    unsigned char opcode;
    Instruction inst;
    int i;

    fprintf(asm_file, "\n; === Entry %04X:%04X ===\n", seg, off);

    while(index < 512) { /* Limite por função */
        opcode = code[index];
        inst = inst_table[opcode];

        if(inst.name == NULL) {
            fprintf(asm_file, "db 0x%02X\n", opcode);
            index++;
            continue;
        }

        /* Bytes */
        fprintf(asm_file, "%04X:%04X  ", seg, off + index);
        for(i = 0; i < inst.length; i++) {
            fprintf(asm_file, "%02X ", code[index + i]);
        }
        fprintf(asm_file, "%-*s", (6 - inst.length) * 3, "");

        /* Mnemônico */
        fprintf(asm_file, "%-6s", inst.name);

        /* Operandos */
        if(inst.format[0] != '\0') {
            decode_operand(code, &index, inst.format[0], seg);
        }
        fprintf(asm_file, "\n");

        index += inst.length;

        if(opcode == 0xC3 || opcode == 0xCB || opcode == 0xCF) break;
    }
}

void scan_ivt(void)
{
    int i;
    unsigned short seg, off;
    unsigned long addr;

    for(i = 0; i < 256; i++) {
        seg = peek(0, (i << 2) + 2);
        off = peek(0, i << 2);
        addr = ((unsigned long)seg << 4) + off;

        if(is_valid_bios_address(addr)) {
            entries[total_entries].int_num = i;
            entries[total_entries].seg = seg;
            entries[total_entries].off = off;
            total_entries++;
        }
    }
}

int main(void)
{
    int i;
    char *filename = "bios_dump.asm";

    asm_file = fopen(filename, "w");
    if(!asm_file) {
        printf("Erro criando arquivo!");
        return 1;
    }

    write_asm_header();
    scan_ivt();

    for(i = 0; i < total_entries; i++) {
        disassemble(entries[i].seg, entries[i].off);
    }

    fclose(asm_file);
    printf("Disassembly salvo em %s\n", filename);
    return 0;
}
