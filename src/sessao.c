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


Sessao* sessao_criar(int id, const char *titulo, const char *horario, int sala, int linhas, int colunas) {
    Sessao *s = (Sessao *) malloc(sizeof(Sessao));
    if (s == NULL) {
        printf("Erro: Falha ao alocar memoria para a sessao!\n");
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
        printf("Erro: memoria insuficiente para alocar linhas no mapa de assentos!\n");
        free(s);
        return NULL;
    }
    
    for (int i=0; i<linhas; i++) {
        s->mapa_assentos[i] = (int *) calloc(colunas, sizeof(int));
        if (s->mapa_assentos[i] == NULL) {
            printf("Erro: memoria insuficiente para alocar colunas no mapa de assentos!\n");
            for (int j = 0; j < i; j++) {
                free(s->mapa_assentos[j]);
            } 
            free(s->mapa_assentos);
            free(s);
            return NULL;
        }
    }
    printf("Sessao criada!\n");
    return s;
}

void sessao_deletar(Sessao *s) {
    if (s == NULL) {
        printf("Erro: nada a ser apagado.\n");
        return;
    } 
    
    if (s->mapa_assentos != NULL) { 
        
        for (int i=0; i<s->linhas; i++) { 
            free(s->mapa_assentos[i]); 
        } 

        free(s->mapa_assentos); 
    } 

    free(s);
    printf("Sessao deletada!\n");
}

