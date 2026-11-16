#ifndef SESSAO_H
#define SESSAO_H

typedef struct sessao Sessao;

Sessao* sessao_criar(int id, char *titulo, char *horario, int sala, int linhas, int colunas);
void sessao_deletar(Sessao *s);

int sessao_obter_id(Sessao *s);
char* sessao_obter_titulo(Sessao *s);
char* sessao_obter_horario(Sessao *s);
int sessao_obter_sala(Sessao *s);
int sessao_obter_linhas(Sessao *s);
int sessao_obter_colunas(Sessao *s);

int sessao_obter_assentos_vagos(Sessao *s);
float sessao_obter_taxa_ocupacao(Sessao *s);
void sessao_exibir_mapa(Sessao *s);

int sessao_assento_livre(Sessao *s, int linha, int coluna);
int sessao_marcar_assento(Sessao *s, int linha, int coluna, int estado);

void sessao_listar_todas(Sessao **sessoes, int qtd_sessoes);
Sessao* sessao_buscar_por_id(Sessao **sessoes, int qtd_sessoes, int id);

#endif