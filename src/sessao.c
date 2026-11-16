#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sessao.h"

struct sessao{
    int id;
    char titulo[50];
    char horario[6];
    int sala;
    int linhas;
    int colunas;
    int **mapa_assentos;
};


Sessao* sessao_criar(int id, char *titulo, char *horario, int sala, int linhas, int colunas) {
    Sessao *s = (Sessao *) malloc(sizeof(Sessao));
    if (s == NULL) {
        return NULL;
    } 
    
    s->id = id;
    s->sala = sala;
    s->linhas = linhas;
    s->colunas = colunas;
    strcpy(s->titulo, titulo);
    strcpy(s->horario, horario);
    
    s->mapa_assentos = (int **) malloc(linhas * sizeof(int *));
    if (s->mapa_assentos == NULL) {
        free(s);
        return NULL;
    }
    
    for (int i=0; i<linhas; i++) {
        s->mapa_assentos[i] = (int *) calloc(colunas, sizeof(int));
        if (s->mapa_assentos[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(s->mapa_assentos[j]);
            } 
            free(s->mapa_assentos);
            free(s);
            return NULL;
        }
    }
    return s;
}

void sessao_deletar(Sessao *s) {
    if (s == NULL) {
        return;
    } 
    
    if (s->mapa_assentos != NULL) { 
        
        for (int i=0; i<s->linhas; i++) { 
            free(s->mapa_assentos[i]); 
        } 

        free(s->mapa_assentos); 
    } 

    free(s);
}

int sessao_obter_id(Sessao *s) { 
    if (s == NULL) { 
        return -1; 
    } 
    return s->id; 
}

int sessao_obter_assentos_vagos(Sessao *s) {
    if (s == NULL || s->mapa_assentos == NULL) {
        return -1;
    }

    int vagos = 0;
    for (int i=0; i<s->linhas; i++) {
        for (int j=0; j<s->colunas; j++) {
            if (s->mapa_assentos[i][j] == 0) {
                vagos++;
            }
        }
    }
    return vagos;
}

Sessao* sessao_buscar_por_id(Sessao **sessoes, int qtd_sessoes, int id) {
    if (sessoes == NULL || qtd_sessoes <= 0) {
        return NULL;
    }

    for (int i=0; i<qtd_sessoes; i++) {
        if (sessoes[i] != NULL && sessao_obter_id(sessoes[i]) == id) {
            return sessoes[i];
        }
    }

    return NULL;
}

float sessao_obter_taxa_ocupacao(Sessao *s) {
    if (s == NULL || s->mapa_assentos == NULL) {
        return -1.0f;
    }
    int total_assentos = s->linhas * s->colunas;

    int vagos = sessao_obter_assentos_vagos(s);
    if (vagos < 0) {
        return -1.0f;
    }

    int ocupados = total_assentos - vagos;
    
    return ((float)ocupados / total_assentos) * 100.0f;
}

int sessao_assento_livre(Sessao *s, int linha, int coluna) {

    if (s == NULL || s->mapa_assentos == NULL) {
        return 0;
    }

    if (linha < 0 || linha >= s->linhas || coluna < 0 || coluna >= s->colunas) {
        return 0;
    }
        
    return (s->mapa_assentos[linha][coluna] == 0);
}

int sessao_marcar_assento(Sessao *s, int linha, int coluna, int estado) {
    if (s == NULL || s->mapa_assentos == NULL) {
        return 0;
    }

    if (linha < 0 || linha >= s->linhas || coluna < 0 || coluna >= s->colunas) {
        return 0;
    }

    if (estado != 0 && estado != 1) {
        return 0;
    }

    s->mapa_assentos[linha][coluna] = estado;
    return 1;
}

void sessao_listar_todas(Sessao **sessoes, int qtd_sessoes) {

    if (sessoes == NULL || qtd_sessoes <= 0) {
        return;
    }

    printf("\n========================== SESSÕES DISPONÍVEIS ==========================\n");
    printf("%-5s | %-22s | %-8s | %-5s | %-12s\n", "ID", "Filme", "Horario", "Sala", "Lugares Vagos");
    printf("-------------------------------------------------------------------------\n");
    
    for (int i = 0; i < qtd_sessoes; i++) {
        if (sessoes[i] != NULL) {
            int vagos = sessao_obter_assentos_vagos(sessoes[i]);
            int total = sessoes[i]->linhas * sessoes[i]->colunas;
            printf("%-5d | %-22s | %-8s | Sala %-2d | %d/%d vagos\n",
                sessoes[i]->id,
                sessoes[i]->titulo,
                sessoes[i]->horario,
                sessoes[i]->sala,
                vagos, total);
        }
    }
    
    printf("============================================================================\n");
}

void sessao_exibir_mapa(Sessao *s) {
    if (s == NULL || s->mapa_assentos == NULL) {
        printf("Erro: sessao nao encontrada!\n");
        return;
    }
    
    printf("\n=== MAPA DE ASSENTOS - SESSÃO %d: %s (%s) ===\n", s->id, s->titulo, s->horario);
    printf(" ================ TELA ================\n\n");
    
    printf("            ");
    for (int j=0; j<s->colunas; j++) {
        printf("%2d ", j+1);
    }
    printf("\n");
    
    for (int i=0; i<s->linhas; i++) {
        printf("Fileira %2d: ", i+1);
        for (int j=0; j<s->colunas; j++) {
            if (s->mapa_assentos[i][j] == 0) {
                printf("[ ]");
            } else {
                printf("[X]");
            }
        }
        printf("\n");
    }
    
    printf("\nLegenda: [ ] Livre | [X] Ocupado/Reservado\n");
}

char* sessao_obter_titulo(Sessao *s) {
    return (s != NULL) ? s->titulo : NULL;
}

char* sessao_obter_horario(Sessao *s) {
    return (s != NULL) ? s->horario : NULL;
}

int sessao_obter_sala(Sessao *s) {
    return (s != NULL) ? s->sala : -1;
}

int sessao_obter_linhas(Sessao *s) {
    return (s != NULL) ? s->linhas : 0;
}

int sessao_obter_colunas(Sessao *s) {
    return (s != NULL) ? s->colunas : 0;
}