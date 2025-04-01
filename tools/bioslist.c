/*
 * # Usando Turbo C++ 3.0
 *   tcc -ml -WX -w- -w-pro -N- -O -G -Z biosdis.c
 *
 *
 *
 */

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>

#define BIOS_SEG 0xF000
#define BIOS_START 0xF0000UL
#define BIOS_END 0xFFFFFUL
#define MAX_ENTRIES 256

typedef struct {
    unsigned short int_num;
    unsigned short seg;
    unsigned short off;
    unsigned long length;
} BiosEntry;

BiosEntry entries[MAX_ENTRIES];
int total_entries = 0;

/* Tabela de opcodes estendida para Pentium */
const char* opcodes[256] = {
    [0x00] = "add", [0x01] = "add", [0x02] = "add", [0x03] = "add",
    [0x04] = "add", [0x05] = "add", [0x08] = "or", [0x09] = "or",
    [0x0B] = "or", [0x10] = "adc", [0x20] = "and", [0x21] = "and",
    [0x22] = "and", [0x30] = "xor", [0x31] = "xor", [0x38] = "cmp",
    [0x39] = "cmp", [0x3A] = "cmp", [0x3B] = "cmp", [0x40] = "inc",
    [0x48] = "dec", [0x50] = "push", [0x58] = "pop", [0x60] = "pusha",
    [0x61] = "popa", [0x68] = "push", [0x6A] = "push", [0x70] = "jo",
    [0x71] = "jno", [0x72] = "jb", [0x73] = "jnb", [0x74] = "jz",
    [0x75] = "jnz", [0x76] = "jbe", [0x77] = "ja", [0x78] = "js",
    [0x79] = "jns", [0x7A] = "jp", [0x7B] = "jnp", [0x7C] = "jl",
    [0x7D] = "jge", [0x7E] = "jle", [0x7F] = "jg", [0x80] = "grp1",
    [0x81] = "grp1", [0x83] = "grp1", [0x88] = "mov", [0x89] = "mov",
    [0x8A] = "mov", [0x8B] = "mov", [0x8C] = "mov", [0x8E] = "mov",
    [0x90] = "nop", [0x98] = "cbw", [0x99] = "cwd", [0x9A] = "call",
    [0xA0] = "mov", [0xA1] = "mov", [0xA2] = "mov", [0xA3] = "mov",
    [0xA4] = "movsb", [0xA5] = "movsw", [0xA6] = "cmpsb", [0xA7] = "cmpsw",
    [0xB0] = "mov", [0xB8] = "mov", [0xC3] = "ret", [0xC9] = "leave",
    [0xCA] = "retf", [0xCB] = "retf", [0xCC] = "int3", [0xCD] = "int",
    [0xCF] = "iret", [0xD1] = "grp2", [0xD3] = "grp2", [0xE4] = "in",
    [0xE6] = "out", [0xE8] = "call", [0xE9] = "jmp", [0xEA] = "jmp",
    [0xEB] = "jmp", [0xEC] = "in", [0xEE] = "out", [0xF4] = "hlt",
    [0xFA] = "cli", [0xFB] = "sti", [0xFC] = "cld", [0xFD] = "std"
};

void print_header() {
    printf("PCAT BIOS Disassembler Pro v2.0\n");
    printf("Target CPU: Pentium S 200MHz\n");
    printf("ROM Area: %04X:%04X-%04X\n\n",
           BIOS_SEG, 0, BIOS_SEG, BIOS_END - (BIOS_SEG << 4));
}

int is_valid_bios_address(unsigned long addr) {
    return (addr >= BIOS_START && addr <= BIOS_END);
}

int get_inst_length(unsigned char *inst) {
    /* Tabela simplificada de comprimentos de instruções */
    switch(inst[0]) {
        case 0x66: case 0x67: // Prefixos
            return 1 + get_inst_length(inst+1);
        case 0x0F: // Two-byte opcodes
            return 2 + (inst[1] >= 0x80 && inst[1] <= 0x8F ? 4 : 0);
        case 0xE8: case 0xE9: // CALL/JMP near
            return 3;
        case 0x9A: case 0xEA: // CALL/JMP far
            return 5;
        case 0xC2: case 0xCA: // RET
            return 3;
        case 0xCD: // INT
            return 2;
        default:
            if (opcodes[inst[0]] != NULL) return 1;
            return 1; // Default para 1 byte
    }
}

void disassemble(unsigned short seg, unsigned short off) {
    unsigned char far *code = MK_FP(seg, off);
    unsigned long addr = ((unsigned long)seg << 4) + off;
    int len, i = 0;

    printf("\n=== BIOS Entry at %04X:%04X ===\n", seg, off);

    while (1) {
        if (!is_valid_bios_address(addr + i) || i >= 4096) break;

        printf("%04X:%04X  ", seg, off + i);

        len = get_inst_length(code + i);

        // Imprime bytes brutos
        for(int j = 0; j < len && j < 6; j++)
            printf("%02X ", code[i+j]);
        for(int j = len; j < 6; j++) printf("   ");

        // Decodifica instrução
        switch(code[i]) {
            case 0x9A: // CALL far
                printf("call far %04X:%04X",
                      *((unsigned short far*)(code + i + 3)),
                      *((unsigned short far*)(code + i + 1)));
                break;
            case 0xEA: // JMP far
                printf("jmp far %04X:%04X",
                      *((unsigned short far*)(code + i + 3)),
                      *((unsigned short far*)(code + i + 1)));
                break;
            case 0xCD: // INT
                printf("int %02X", code[i+1]);
                break;
            case 0xE8: // CALL near
                printf("call near %04X", *((unsigned short far*)(code + i + 1)));
                break;
            case 0xC3: // RET
                printf("ret");
                if (len > 1) break; // Não continuar após RET
                return;
            case 0xCF: // IRET
                printf("iret");
                return;
            default:
                if (opcodes[code[i]] != NULL)
                    printf("%s", opcodes[code[i]]);
                else
                    printf("db %02X", code[i]);
        }
        printf("\n");

        i += len;

        // Verifica instruções de término
        if (code[i-1] == 0xCF || code[i-1] == 0xC3 || code[i-1] == 0xCB)
            break;
    }
}

void scan_ivt() {
    printf("Scanning IVT for BIOS entries...\n");

    for(int i = 0; i < 256; i++) {
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

void full_disassembly() {
    printf("\nStarting Full Disassembly...\n");

    for(int i = 0; i < total_entries; i++) {
        disassemble(entries[i].seg, entries[i].off);
    }
}

void verify_signatures() {
    unsigned char far *reset_vec = MK_FP(0xF000, 0xFFF0);

    printf("\nVerifying BIOS signatures...\n");
    if (memcmp(reset_vec, "\xEA\x5B\xE0\x00\xF0", 5) == 0) {
        printf("Valid PCAT BIOS signature found at F000:FFF0\n");
    }
}

int main() {
    clrscr();
    print_header();
    verify_signatures();
    scan_ivt();
    full_disassembly();

    printf("\nDisassembly completed. Entries found: %d\n", total_entries);
    printf("Press any key to exit...");
    getch();
    return 0;
}
