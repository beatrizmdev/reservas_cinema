#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "sessao.h"
#include "reserva.h"

/*
 * Salva a lista de sessoes em arquivo de texto.
 * Formato por linha: ID;TITULO;HORARIO;SALA;LINHAS;COLUNAS
 * Retorna 1 em caso de sucesso ou 0 se houver erro ao abrir o arquivo.
 */
int persistencia_salvar_sessoes(char *nome_arquivo, Sessao **sessoes, int qtd) {
    if (nome_arquivo == NULL || sessoes == NULL) {
        return 0;
    }

    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo '%s' para escrita.\n", nome_arquivo);
        return 0;
    }

    fprintf(f, "%d\n", qtd);

    for (int i = 0; i < qtd; i++) {
        if (sessoes[i] != NULL) {
            fprintf(f, "%d;%s;%s;%d;%d;%d\n",
                    sessao_obter_id(sessoes[i]),
                    sessao_obter_titulo(sessoes[i]),
                    sessao_obter_horario(sessoes[i]),
                    sessao_obter_sala(sessoes[i]),
                    sessao_obter_linhas(sessoes[i]),
                    sessao_obter_colunas(sessoes[i]));
        }
    }

    fclose(f);
    return 1;
}


int persistencia_carregar_sessoes(char *nome_arquivo, Sessao **sessoes, int max_sessoes) {
    if (nome_arquivo == NULL || sessoes == NULL || max_sessoes <= 0) {
        return 0;
    }

    FILE *f = fopen(nome_arquivo, "r");
    if (f == NULL) {
        return 0;
    }

    int total_no_arquivo = 0;
    if (fscanf(f, "%d\n", &total_no_arquivo) != 1) {
        fclose(f);
        return 0;
    }

    int carregadas = 0;
    int id, sala, linhas, colunas;
    char titulo[100];
    char horario[10];

    while (carregadas < max_sessoes && fscanf(f, "%d;%99[^;];%9[^;];%d;%d;%d\n", &id, titulo, horario, &sala, &linhas, &colunas) == 6) {
        sessoes[carregadas] = sessao_criar(id, titulo, horario, sala, linhas, colunas);
        if (sessoes[carregadas] != NULL) {
            carregadas++;
        }
    }

    fclose(f);
    return carregadas;
}


int persistencia_salvar_reservas(char *nome_arquivo, Reserva **reservas, int qtd) {
    if (nome_arquivo == NULL || reservas == NULL) {
        return 0;
    }

    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo '%s' para escrita.\n", nome_arquivo);
        return 0;
    }

    fprintf(f, "%d\n", qtd);

    for (int i = 0; i < qtd; i++) {
        if (reservas[i] != NULL) {
            fprintf(f, "%d;%d;%s;%s;%d;%d;%d\n",
                    reserva_obter_codigo(reservas[i]),
                    reserva_obter_id_sessao(reservas[i]),
                    reserva_obter_cliente(reservas[i]),
                    reserva_obter_cpf(reservas[i]),
                    reserva_obter_linha(reservas[i]),
                    reserva_obter_coluna(reservas[i]),
                    reserva_esta_ativa(reservas[i]));
        }
    }

    fclose(f);
    return 1;
}


int persistencia_carregar_reservas(char *nome_arquivo, Reserva **reservas, int max_reservas, Sessao **sessoes, int qtd_sessoes) {
    if (nome_arquivo == NULL || reservas == NULL || max_reservas <= 0) {
        return 0;
    }

    FILE *f = fopen(nome_arquivo, "r");
    if (f == NULL) {
        return 0;
    }

    int total_no_arquivo = 0;
    if (fscanf(f, "%d\n", &total_no_arquivo) != 1) {
        fclose(f);
        return 0;
    }

    int carregadas = 0;
    int codigo, id_sessao, linha, coluna, ativa;
    char cliente[100];
    char cpf[20];

    while (carregadas < max_reservas && fscanf(f, "%d;%d;%99[^;];%19[^;];%d;%d;%d\n", &codigo, &id_sessao, cliente, cpf, &linha, &coluna, &ativa) == 7) {
        Reserva *r = reserva_criar(codigo, id_sessao, cliente, cpf, linha, coluna);
        if (r != NULL) {
            if (!ativa) {
                reserva_cancelar(r, NULL);
            } else {
                if (sessoes != NULL) {
                    for (int s = 0; s < qtd_sessoes; s++) {
                        if (sessoes[s] != NULL && sessao_obter_id(sessoes[s]) == id_sessao) {
                            sessao_marcar_assento(sessoes[s], linha, coluna, 1);
                            break;
                        }
                    }
                }
            }

            reservas[carregadas] = r;
            carregadas++;
        }
    }

    fclose(f);
    return carregadas;
}