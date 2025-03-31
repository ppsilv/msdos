#include <stdio.h>
#include <stdlib.h>

void banner(){
    printf("RETRO: Open Software Copyright (C) 2025\n");
    printf("Autor: Paulo da Silva (pgordao)\n");
#ifndef __GNUC__
    printf("Versao: 0.1.0 para TurboC\n\n");
#else
    printf("Versao: 0.1.0 para GCC\n\n");
#endif
}

int main(int argc, char *argv[]) {
    FILE *arquivo_entrada, *arquivo_saida;
    int caractere, anterior = EOF;
    char temp_nome[] = "TEMPXXXX.TMP";

    banner();

    if (argc != 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    /* Abre arquivo original para leitura binária */
    if ((arquivo_entrada = fopen(argv[1], "rb")) == NULL) {
        printf("Erro ao abrir arquivo: %s\n", argv[1]);
        return 1;
    }

    /* Cria arquivo temporário para escrita binária */
    if ((arquivo_saida = fopen(temp_nome, "wb")) == NULL) {
        printf("Erro ao criar arquivo temporario\n");
        fclose(arquivo_entrada);
        return 1;
    }

    /* Processa cada caractere */
    while ((caractere = fgetc(arquivo_entrada)) != EOF) {
        if (caractere == '\n') {
            /* Se encontrar LF sem CR anterior, adiciona CR */
            if (anterior != '\r') {
                fputc('\r', arquivo_saida);
            }
            fputc('\n', arquivo_saida);
        } else {
            fputc(caractere, arquivo_saida);
        }
        anterior = caractere;
    }

    /* Fecha ambos os arquivos */
    fclose(arquivo_entrada);
    fclose(arquivo_saida);

    /* Remove o original e renomeia o temporário */
    if (remove(argv[1]) != 0) {
        printf("Erro ao remover arquivo original\n");
        remove(temp_nome);
        return 1;
    }

    if (rename(temp_nome, argv[1]) != 0) {
        printf("Erro ao renomear arquivo temporario\n");
        return 1;
    }

    printf("Conversao do arquivo %s concluida com sucesso!\n",argv[1]);
    return 0;
}
