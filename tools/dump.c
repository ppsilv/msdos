#include <stdio.h>
#include <ctype.h>


#ifndef __GNUC__
#include <conio.h>  // Para getch()
#endif

void banner(){
    printf("RETRO: Open Software Copyright (C) 2025\n");
    printf("Autor: Paulo da Silva (pgordao)\n");
#ifndef __GNUC__
    printf("Versao: 0.1.0 para TurboC\n");
#else
    printf("Versao: 0.1.0 para GCC\n");
#endif
}

int main(int argc, char *argv[]) {
    FILE *arquivo;
    unsigned char buffer[16];
    int bytes_lidos, i, linha = 0;
    unsigned long endereco = 0L;

    banner();

    if (argc != 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    if ((arquivo = fopen(argv[1], "rb")) == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    printf("\nDump do arquivo: %s\n\n", argv[1]);

    while ((bytes_lidos = fread(buffer, 1, 16, arquivo)) > 0) {
        /* Imprime endereço */
        printf("%08lX: ", endereco);

        /* Hex dump (16 bytes) */
        for (i = 0; i < 16; i++) {
            if (i < bytes_lidos)
                printf("%02X ", buffer[i]);
            else
                printf("   ");
        }

        /* ASCII (8 caracteres) */
        printf(" ");
        for (i = 0; i < 8; i++) {
            if (i < bytes_lidos) {
                if (isprint(buffer[i]) && buffer[i] < 0x80)
                    putchar(buffer[i]);
                else
                    putchar('.');
            }
            else
                putchar(' ');
        }

        printf("\n");
        endereco += 16;

        /* Controle de paginação */
        if (++linha % 16 == 0) {
            printf("\nPressione qualquer tecla para continuar...");
#ifndef __GNUC__
            getch();
#else
            getchar();
#endif
            printf("\n");
        }
    }

    fclose(arquivo);
    return 0;
}
