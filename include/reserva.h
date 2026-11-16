#ifndef RESERVA_H
#define RESERVA_H

#include "sessao.h"

typedef struct reserva Reserva;

Reserva* reserva_criar(int codigo, int id_sessao, char *nome_cliente, char *cpf, int linha, int coluna);
void reserva_deletar(Reserva *r);

int reserva_obter_codigo(Reserva *r);
int reserva_obter_id_sessao(Reserva *r);
char* reserva_obter_cliente(Reserva *r);
char* reserva_obter_cpf(Reserva *r);
int reserva_obter_linha(Reserva *r);
int reserva_obter_coluna(Reserva *r);
int reserva_esta_ativa(Reserva *r);

int reserva_cancelar(Reserva *r, Sessao *s);
int reserva_cancelar_por_sessao(Reserva **reservas, int qtd_reservas, Sessao *s);

void reserva_listar_todas(Reserva **reservas, int qtd);
Reserva* reserva_buscar_por_codigo(Reserva **reservas, int qtd, int codigo);

#endif