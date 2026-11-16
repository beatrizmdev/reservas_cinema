#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "sessao.h"
#include "reserva.h"

int persistencia_salvar_sessoes(char *nome_arquivo, Sessao **sessoes, int qtd);
int persistencia_salvar_reservas(char *nome_arquivo, Reserva **reservas, int qtd);

int persistencia_carregar_sessoes(char *nome_arquivo, Sessao **sessoes, int max_sessoes);
int persistencia_carregar_reservas(char *nome_arquivo, Reserva **reservas, int max_reservas, Sessao **sessoes, int qtd_sessoes);

#endif