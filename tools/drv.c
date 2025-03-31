#include <dos.h>
#include <stdio.h>
#include <alloc.h>

int main() {
    FILE *fp=0;
    char far *buffer;
    int setor, resultado;
    struct SREGS segregs;
    union REGS inregs, outregs;

    /* Aloca buffer para 18 setores (tamanho total = 18*512 = 9216 bytes ) */
    buffer = (char far *)farmalloc(18 * 512);

    if(!buffer) {
	printf("Erro na alocacao de memoria!\n");
	return 1;
    }
     /* Prepara os parametros para a interrupcao 13h */
     inregs.h.ah = 0x02;    /* Funcao de leitura de setores */
     inregs.h.al = 18;      /* Numero de setores a ler */
     inregs.h.ch = 0;        /* Trilha 0 */
     inregs.h.cl = 1;        /* Primeiro setor */
     inregs.h.dh = 0;        /* Cabecote 0 */
     inregs.h.dl = 0x00;     /* Drive A: */
     inregs.x.bx = FP_OFF(buffer);  /* Offset do buffer */

     /* Configura registros de segmento */
     segregs.es = FP_SEG(buffer);    /* Segmento do buffer */

     /* Chama a interrupcao BIOS */
     int86x(0x13, &inregs, &outregs, &segregs);

      /* Verifica se houve erro (flag de carry ativada) */
      if(outregs.x.cflag) {
	printf("Erro na leitura! Codigo: 0x%02X\n", outregs.h.ah);
      } else {
	printf("Trilha 0 lida com sucesso!\n");

	/* Salva os dados em um arquivo */
	fp = fopen("TRACK0.DMP", "wb");
	if(fp) {
	    for(setor = 0; setor < 18 * 512; setor++) {
	       fputc(buffer[setor],fp);
	    }
	    fclose(fp);
	    printf("Dados salvos em TRACK0.DMP\n");
	} else {
	    printf("Erro ao criar arquivo!\n");
	}
    }
    farfree(buffer);
    return 0;
}
