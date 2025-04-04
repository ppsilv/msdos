#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <alloc.h>

#define SECTOR_SIZE 512
#define FLOPPY_DRIVE 0x00

    int comando, cylinder, head, sector, initial_trilha, final_trilha, vezes;
    char far *buffer;
    char filename[13];
    FILE *fp;
    union REGS inregs, outregs;
    struct SREGS segregs;

void comandoA(){
                 printf("\nLeitura de setor\n");
                printf("Cilindro (0-79): ");
                scanf("%d", &cylinder);
                printf("Cabeca (0-1): ");
                scanf("%d", &head);
                printf("Setor (1-18): ");
                scanf("%d", &sector);

                if(cylinder < 0 || cylinder > 79 || head < 0 || head > 1 || sector < 1 || sector > 18) {
                    printf("Parametros invalidos!");
                    break;
                }

                // Reset do controlador
                inregs.h.ah = 0x00;
                inregs.h.dl = FLOPPY_DRIVE;
                int86(0x13, &inregs, &outregs);

                // Leitura dummy para posicionamento
                inregs.h.ah = 0x02;
                inregs.h.al = 1;
                inregs.h.ch = cylinder;
                inregs.h.cl = sector;
                inregs.h.dh = head;
                inregs.h.dl = FLOPPY_DRIVE;
                inregs.x.bx = FP_OFF(buffer);
                segregs.es = FP_SEG(buffer);
                segread(&segregs);

                int86x(0x13, &inregs, &outregs, &segregs);

                if(outregs.x.cflag || outregs.h.ah) {
                    printf("Erro %02X no posicionamento!", outregs.h.ah);
                } else {
                    sprintf(filename, "C:\\C%02dH%dS%02d.BIN", cylinder, head, sector);
                    fp = fopen(filename, "wb");
                    if(fp) {
                        fwrite(buffer, SECTOR_SIZE, 1, fp);
                        fclose(fp);
                        printf("Dados salvos em %s", filename);
                    } else {
                        printf("Erro criando arquivo!");
                    }
                }

}

void comandoB(){
                printf("\nBusca de trilha\n");
                printf("Cilindro (0-79): ");
                scanf("%d", &cylinder);
                printf("Cabeca (0-1): ");
                scanf("%d", &head);

                if(cylinder < 0 || cylinder > 79 || head < 0 || head > 1) {
                    printf("Parametros invalidos!");
                    break;
                }

                // Reset do controlador
                inregs.h.ah = 0x00;
                inregs.h.dl = FLOPPY_DRIVE;
                int86(0x13, &inregs, &outregs);

                // Leitura dummy para posicionamento
                inregs.h.ah = 0x02;
                inregs.h.al = 0;       // Zero setores (apenas posiciona)
                inregs.h.ch = cylinder;
                inregs.h.cl = 1;       // Setor arbitrário
                inregs.h.dh = head;
                inregs.h.dl = FLOPPY_DRIVE;
                inregs.x.bx = FP_OFF(buffer);
                segregs.es = FP_SEG(buffer);
                segread(&segregs);

                int86x(0x13, &inregs, &outregs, &segregs);

                if(outregs.x.cflag || outregs.h.ah) {
                    printf("Erro %02X no posicionamento!", outregs.h.ah);
                } else {
                    printf("Cabeca posicionada em Cilindro %d Cabeca %d", cylinder, head);
                }
}

void comandoC(){
    int i=0;
                printf("\nTeste de movimento da cabeca\n");
                printf("Cabeca (0-1): ");
                scanf("%d", &head);
                printf("Trilha inicial (0-79): ");
                scanf("%d", &initial_trilha);
                printf("Trilha final (0-79): ");
                scanf("%d", &final_trilha);
                printf("Quantidade de ciclos: ");
                scanf("%d", &vezes);

                // Validação
                if(head < 0 || head > 1 || initial_trilha < 0 || initial_trilha > 79 ||
                   final_trilha < 0 || final_trilha > 79 || vezes < 1) {
                    printf("Parametros invalidos!");
                    break;
                }

                // Loop principal de movimento
                for( i = 0; i < vezes; i++) {
                    // Alternar entre as trilhas
                    int target_trilha = (i % 2 == 0) ? initial_trilha : final_trilha;

                    // Reset do controlador
                    inregs.h.ah = 0x00;
                    inregs.h.dl = FLOPPY_DRIVE;
                    int86(0x13, &inregs, &outregs);

                    // Movimento para trilha alvo
                    inregs.h.ah = 0x02;     // Função de leitura
                    inregs.h.al = 0;        // Zero setores (apenas posiciona)
                    inregs.h.ch = target_trilha;
                    inregs.h.cl = 1;        // Setor arbitrário
                    inregs.h.dh = head;
                    inregs.h.dl = FLOPPY_DRIVE;
                    inregs.x.bx = FP_OFF(buffer);
                    segregs.es = FP_SEG(buffer);
                    segread(&segregs);

                    int86x(0x13, &inregs, &outregs, &segregs);

                    if(outregs.x.cflag || outregs.h.ah) {
                        printf("\nErro %02X no movimento %d!", outregs.h.ah, i+1);
                        break;
                    }
                    printf("\nCiclo %d: Trilha %d Cabeca %d", i+1, target_trilha, head);
                }

}

void main() {


    buffer = farmalloc(SECTOR_SIZE);
    if(!buffer) {
        printf("Erro alocando memoria!");
        return;
    }

    while(1) {
        clrscr();
        printf("Comandos:\nA - Ler setor\nB - Buscar trilha\nC - Teste de movimento\nQ - Sair\n\nComando: ");

        comando = getch();
        printf("%c\n", toupper(comando));
        comando = toupper(comando);

        switch(comando) {
            case 'A': {
                comandoA();
                break;
            }

            case 'B': {
                comandoB();
                break;
            }

            case 'C': {
                comandoC();
                break;
            }

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
