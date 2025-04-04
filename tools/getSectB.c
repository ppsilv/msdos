#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <alloc.h>

#define SECTOR_SIZE 512
#define FLOPPY_DRIVE 0x00

/* Protótipos de funções */
void floppy_reset(void);
int floppy_seek(int cylinder, int head);
int read_sector(int cylinder, int head, int sector, char far *buffer);
int seek_track(int cylinder, int head);
void head_stress_test(int head, int initial_trk, int final_trk, int cycles);
void display_menu(void);

/* Função principal */
void main() {
    int comando, cylinder, head, sector, trk_inicial, trk_final, ciclos;
    char far *buffer = farmalloc(SECTOR_SIZE);

    if(!buffer) {
        printf("Erro alocando memoria!");
        return;
    }

    while(1) {
        clrscr();
        display_menu();
        comando = toupper(getch());
        printf("%c\n", comando);

        switch(comando) {
            case 'A':
                printf("\nCilindro (0-79): "); scanf("%d", &cylinder);
                printf("Cabeca (0-1): "); scanf("%d", &head);
                printf("Setor (1-18): "); scanf("%d", &sector);

                if(read_sector(cylinder, head, sector, buffer) == 0) {
                    printf("Setor lido com sucesso!");
                }
                break;

            case 'B':
                printf("\nCilindro (0-79): "); scanf("%d", &cylinder);
                printf("Cabeca (0-1): "); scanf("%d", &head);

                if(seek_track(cylinder, head) == 0) {
                    printf("Posicionamento realizado!");
                }
                break;

            case 'C':
                printf("\nCabeca (0-1): "); scanf("%d", &head);
                printf("Trilha inicial: "); scanf("%d", &trk_inicial);
                printf("Trilha final: "); scanf("%d", &trk_final);
                printf("Ciclos: "); scanf("%d", &ciclos);

                head_stress_test(head, trk_inicial, trk_final, ciclos);
                break;

            case 'Q':
                farfree(buffer);
                return;

            default:
                printf("Comando invalido!");
                break;
        }
        printf("\nPressione uma tecla...");
        getch();
    }
}

/* Funções implementadas */
void floppy_reset(void) {
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.dl = FLOPPY_DRIVE;
    int86(0x13, &regs, &regs);
}

int floppy_seek(int cylinder, int head) {
    union REGS regs;
    struct SREGS sregs;
    char far *dummy = farmalloc(SECTOR_SIZE);
    int result;

    regs.h.ah = 0x02;
    regs.h.al = 0;
    regs.h.ch = cylinder;
    regs.h.cl = 1;
    regs.h.dh = head;
    regs.h.dl = FLOPPY_DRIVE;
    regs.x.bx = FP_OFF(dummy);
    sregs.es = FP_SEG(dummy);

    int86x(0x13, &regs, &regs, &sregs);
    farfree(dummy);

    return (regs.x.cflag || regs.h.ah) ? -1 : 0;
}

int read_sector(int cylinder, int head, int sector, char far *buffer) {
    union REGS regs;
    struct SREGS sregs;
    char filename[13];
    FILE *fp;
    int result;

    /* Validação */
    if(cylinder < 0 || cylinder > 79 || head < 0 || head > 1 || sector < 1 || sector > 18) {
        printf("Parametros invalidos!");
        return -1;
    }

    floppy_reset();

    regs.h.ah = 0x02;
    regs.h.al = 1;
    regs.h.ch = cylinder;
    regs.h.cl = sector;
    regs.h.dh = head;
    regs.h.dl = FLOPPY_DRIVE;
    regs.x.bx = FP_OFF(buffer);
    sregs.es = FP_SEG(buffer);

    int86x(0x13, &regs, &regs, &sregs);

    if(regs.x.cflag || regs.h.ah) {
        printf("Erro %02X na leitura!", regs.h.ah);
        return -1;
    }

    /* Grava arquivo */
    sprintf(filename, "C:\\C%02dH%1dS%02d.BIN", cylinder, head, sector);
    if((fp = fopen(filename, "wb")) == NULL) {
        printf("Erro criando arquivo!");
        return -1;
    }
    fwrite(buffer, SECTOR_SIZE, 1, fp);
    fclose(fp);

    return 0;
}

int seek_track(int cylinder, int head) {
    int result;

    if(cylinder < 0 || cylinder > 79 || head < 0 || head > 1) {
        printf("Parametros invalidos!");
        return -1;
    }

    floppy_reset();
    result = floppy_seek(cylinder, head);

    if(result != 0) {
        printf("Erro no posicionamento!");
        return -1;
    }
    return 0;
}

void head_stress_test(int head, int initial_trk, int final_trk, int cycles) {
    int i, current_trk;

    if(head < 0 || head > 1 || initial_trk < 0 || initial_trk > 79 ||
       final_trk < 0 || final_trk > 79 || cycles < 1) {
        printf("Parametros invalidos!");
        return;
    }

    for(i = 0; i < cycles; i++) {
        current_trk = (i % 2) ? final_trk : initial_trk;

        if(floppy_seek(current_trk, head) != 0) {
            printf("\nFalha no ciclo %d", i+1);
            return;
        }
        printf("\nCiclo %04d/%04d - Trilha: %02d", i+1, cycles, current_trk);
    }
}

void display_menu(void) {
    printf("=== Controle Floppy Drive ==="
           "\n[A] Ler setor"
           "\n[B] Posicionar cabeca"
           "\n[C] Teste de stress"
           "\n[Q] Sair"
           "\n\nComando: ");
}
