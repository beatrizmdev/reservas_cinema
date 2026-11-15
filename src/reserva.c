#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reserva.h"
#include "sessao.h"

struct reserva {
    int codigo;
    int id_sessao;
    char nome_cliente[100];
    char cpf[20];
    int linha;
    int coluna;
    int ativa;
};

Reserva* reserva_criar(int codigo, int id_sessao, char *nome_cliente, char *cpf, int linha, int coluna) {
    if (nome_cliente == NULL || cpf == NULL) {
        return NULL;
    }

    Reserva *r = (Reserva*) malloc(sizeof(Reserva));
    if (r == NULL) {
        return NULL;
    }

    r->codigo = codigo;
    r->id_sessao = id_sessao;

    strncpy(r->nome_cliente, nome_cliente, sizeof(r->nome_cliente) - 1);
    r->nome_cliente[sizeof(r->nome_cliente) - 1] = '\0';

    strncpy(r->cpf, cpf, sizeof(r->cpf) - 1);
    r->cpf[sizeof(r->cpf) - 1] = '\0';

    r->linha = linha;
    r->coluna = coluna;
    r->ativa = 1;

    return r;
}

void reserva_deletar(Reserva *r) {
    if (r != NULL) {
        free(r);
    } else { printf("\n[Erro]: Reserva não encontrada\n");}
}

int reserva_obter_codigo(Reserva *r) {
    return (r != NULL) ? r->codigo : -1;
}

int reserva_obter_id_sessao(Reserva *r) {
    return (r != NULL) ? r->id_sessao : -1;
}

char* reserva_obter_cliente(Reserva *r) {
    return (r != NULL) ? r->nome_cliente : "\n[Erro]: cliente não encontrado\n";
}

char* reserva_obter_cpf(Reserva *r) {
    return (r != NULL) ? r->cpf : "\n[Erro]: cliente não encontrado\n";
}

int reserva_obter_linha(Reserva *r) {
    return (r != NULL) ? r->linha : -1;
}

int reserva_obter_coluna(Reserva *r) {
    return (r != NULL) ? r->coluna : -1;
}

int reserva_esta_ativa(Reserva *r) {
    return (r != NULL) ? r->ativa : 0;
}

int reserva_cancelar(Reserva *r, Sessao *s) {
    if (r == NULL || !r->ativa) {
        return 0;
    }

    r->ativa = 0;

    if (s != NULL) {
        sessao_marcar_assento(s, r->linha, r->coluna, 0);
    } else {return 0;}

    return 1;
}

void reserva_listar_todas(Reserva **reservas, int qtd) {
    if (reservas == NULL || qtd <= 0) {
        printf("\n[Erro]: nenhuma reserva registrada.\n");
        return;
    }

    printf("\n=============================== RESERVAS ===============================\n");
    printf("%-7s | %-9s | %-20s | %-14s | %-7s | %-9s\n",
           "Cod", "Sessao", "Cliente", "CPF", "Assento", "Status");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        if (reservas[i] != NULL) {
            printf("%-7d | %-9d | %-20s | %-14s | F%-2d C%-2d | %-9s\n",
                   reservas[i]->codigo,
                   reservas[i]->id_sessao,
                   reservas[i]->nome_cliente,
                   reservas[i]->cpf,
                   reservas[i]->linha + 1,
                   reservas[i]->coluna + 1,
                   reservas[i]->ativa ? "Ativa" : "Cancelada");
        }
    }
    printf("========================================================================\n");
}

Reserva* reserva_buscar_por_codigo(Reserva **reservas, int qtd, int codigo) {
    if (reservas == NULL || qtd <= 0) {
        return NULL;
    }

    for (int i = 0; i < qtd; i++) {
        if (reservas[i] != NULL && reservas[i]->codigo == codigo) {
            return reservas[i];
        }
    }
    return NULL;
}