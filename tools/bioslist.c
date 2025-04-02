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

/* Tabela de opcodes para 8086/ Pentium compatível com 16-bit */
const char *opcodes[256] = {
    /* 0x00 */ "add", "add", "add", "add", "add", "add", "add", "add",
    /* 0x08 */ "or",  "or",  "or",  "or",  "or",  "or",  "or",  "or",
    /* 0x10 */ "adc", "adc", "adc", "adc", "adc", "adc", "adc", "adc",
    /* 0x18 */ "sbb", "sbb", "sbb", "sbb", "sbb", "sbb", "sbb", "sbb",
    /* 0x20 */ "and", "and", "and", "and", "and", "and", "and", "and",
    /* 0x28 */ "sub", "sub", "sub", "sub", "sub", "sub", "sub", "sub",
    /* 0x30 */ "xor", "xor", "xor", "xor", "xor", "xor", "xor", "xor",
    /* 0x38 */ "cmp", "cmp", "cmp", "cmp", "cmp", "cmp", "cmp", "cmp",
    /* 0x40 */ "inc", "inc", "inc", "inc", "inc", "inc", "inc", "inc",
    /* 0x48 */ "dec", "dec", "dec", "dec", "dec", "dec", "dec", "dec",
    /* 0x50 */ "push","push","push","push","push","push","push","push",
    /* 0x58 */ "pop", "pop", "pop", "pop", "pop", "pop", "pop", "pop",
    /* 0x60 */ "pusha","popa","bound","arpl","fs","gs","opsize","addrsize",
    /* 0x68 */ "push","imul","push","imul","insb","insw","outsb","outsw",
    /* 0x70 */ "jo",  "jno", "jb",  "jnb", "jz",  "jnz", "jbe", "ja",
    /* 0x78 */ "js",  "jns", "jp",  "jnp", "jl",  "jge", "jle", "jg",
    /* 0x80 */ "grp1","grp1","grp1","grp1","test","test","xchg","xchg",
    /* 0x88 */ "mov", "mov", "mov", "mov", "mov", "mov", "mov", "mov",
    /* 0x90 */ "nop", "xchg","xchg","xchg","xchg","xchg","xchg","xchg",
    /* 0x98 */ "cbw", "cwd", "call","fwait","pushf","popf", "sahf","lahf",
    /* 0xA0 */ "mov", "mov", "mov", "mov", "movsb","movsw","cmpsb","cmpsw",
    /* 0xA8 */ "test","test","stosb","stosw","lodsb","lodsw","scasb","scasw",
    /* 0xB0 */ "mov", "mov", "mov", "mov", "mov", "mov", "mov", "mov",
    /* 0xB8 */ "mov", "mov", "mov", "mov", "mov", "mov", "mov", "mov",
    /* 0xC0 */ "grp2","grp2","ret", "ret", "les", "lds", "mov", "mov",
    /* 0xC8 */ "enter","leave","retf","retf","int3","int", "into","iret",
    /* 0xD0 */ "grp2","grp2","grp2","grp2","aam", "aad", "salc","xlat",
    /* 0xD8 */ "esc", "esc", "esc", "esc", "esc", "esc", "esc", "esc",
    /* 0xE0 */ "loopne","loope","loop","jcxz","in", "out","call","jmp",
    /* 0xE8 */ "jmp", "jmp", "in", "out","call","jmp","jmp","jmp",
    /* 0xF0 */ "lock","repne","rep", "hlt", "cmc","grp3a","grp3b","grp3c",
    /* 0xF8 */ "clc", "stc", "cli", "sti", "cld", "std", "grp4","grp5"
};

void print_header(void)
{
    printf("PCAT BIOS Disassembler Pro v2.0\n");
    printf("Target CPU: Pentium S 200MHz\n");
    printf("ROM Area: %04X:%04X-%04X\n\n",
           BIOS_SEG, 0, BIOS_SEG, BIOS_END - (BIOS_SEG << 4));
}

int is_valid_bios_address(unsigned long addr)
{
    return (addr >= BIOS_START && addr <= BIOS_END);
}

int get_inst_length(unsigned char far *inst)
{
    unsigned char op = *inst;
    int len = 1;

    if(op == 0x66 || op == 0x67) { /* Prefixos 16/32 bits */
        op = *(inst+1);
        len++;
    }

    switch(op) {
        case 0x0F: /* Two-byte opcodes */
            len++;
            if(*(inst+len-1) >= 0x80 && *(inst+len-1) <= 0x8F) len += 4;
            break;
        case 0xE8: case 0xE9: len += 2; break; /* CALL/JMP near */
        case 0x9A: case 0xEA: len += 4; break; /* CALL/JMP far */
        case 0xC2: case 0xCA: len += 2; break; /* RET */
        case 0xCD: len += 1; break;            /* INT */
        default:
            if(op >= 0xB0 && op <= 0xBF) len += 0; /* MOV immediate */
            else if(op >= 0x70 && op <= 0x7F) len += 1; /* Jcc short */
    }
    return len;
}

void disassemble(unsigned short seg, unsigned short off)
{
    unsigned char far *code = MK_FP(seg, off);
    unsigned long addr = ((unsigned long)seg << 4) + off;
    int len, i = 0, j;

    printf("\n=== BIOS Entry at %04X:%04X ===\n", seg, off);

    while(1) {
        if(!is_valid_bios_address(addr + i) || i >= 4096) break;

        printf("%04X:%04X  ", seg, off + i);

        len = get_inst_length(code + i);

        /* Imprime bytes brutos */
        for(j = 0; j < len && j < 6; j++)
            printf("%02X ", code[i+j]);
        for(; j < 6; j++) printf("   ");

        /* Decodifica instrução */
        switch(code[i]) {
            case 0x9A: /* CALL far */
                printf("call far %04X:%04X",
                      *((unsigned short far*)(code + i + 3)),
                      *((unsigned short far*)(code + i + 1)));
                break;
            case 0xEA: /* JMP far */
                printf("jmp far %04X:%04X",
                      *((unsigned short far*)(code + i + 3)),
                      *((unsigned short far*)(code + i + 1)));
                break;
            case 0xCD: /* INT */
                printf("int %02X", code[i+1]);
                break;
            case 0xE8: /* CALL near */
                printf("call near %04X", *((unsigned short far*)(code + i + 1)));
                break;
            case 0xC3: /* RET */
                printf("ret");
                return;
            case 0xCF: /* IRET */
                printf("iret");
                return;
            default:
                if(opcodes[code[i]][0] != '\0')
                    printf("%s", opcodes[code[i]]);
                else
                    printf("db %02X", code[i]);
        }
        printf("\n");

        i += len;

        if(code[i-1] == 0xCF || code[i-1] == 0xC3 || code[i-1] == 0xCB)
            break;
    }
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
