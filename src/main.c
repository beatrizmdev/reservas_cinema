#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sessao.h"
#include "reserva.h"
#include "persistencia.h"

#define MAX_SESSOES 50
#define MAX_RESERVAS 200
#define ARQ_SESSOES "dados/sessoes.txt"
#define ARQ_RESERVAS "dados/reservas.txt"

static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void ler_texto(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

int main(void) {
    Sessao *sessoes[MAX_SESSOES] = { NULL };
    Reserva *reservas[MAX_RESERVAS] = { NULL };
    int qtd_sessoes = 0;
    int qtd_reservas = 0;

    printf("\n=======================================================\n");
    printf("         CINE-C: SISTEMA DE GESTAO DE CINEMA          \n");
    printf("=======================================================\n");

    qtd_sessoes = persistencia_carregar_sessoes(ARQ_SESSOES, sessoes, MAX_SESSOES);
    qtd_reservas = persistencia_carregar_reservas(ARQ_RESERVAS, reservas, MAX_RESERVAS, sessoes, qtd_sessoes);

    printf("[INFO] %d sessao(oes) e %d reserva(s) recuperadas do disco.\n", qtd_sessoes, qtd_reservas);

    int opcao = -1;

    do {
        printf("\n------------------- MENU PRINCIPAL --------------------\n");
        printf("1. Cadastrar nova sessao\n");
        printf("2. Listar todas as sessoes\n");
        printf("3. Visualizar mapa de assentos de uma sessao\n");
        printf("4. Realizar uma reserva\n");
        printf("5. Cancelar uma reserva\n");
        printf("6. Listar todas as reservas\n");
        printf("7. Salvar dados agora\n");
        printf("0. Sair do sistema\n");
        printf("-------------------------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            printf("[AVISO] Entrada invalida. Digite um numero.\n");
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1: {
                if (qtd_sessoes >= MAX_SESSOES) {
                    printf("[ERRO] Limite maximo de sessoes atingido!\n");
                    break;
                }

                int id, sala, linhas, colunas;
                char titulo[100];
                char horario[10];

                printf("\n--- Cadastrar Nova Sessao ---\n");
                printf("ID da Sessao: ");
                scanf("%d", &id);
                limpar_buffer();

                if (sessao_buscar_por_id(sessoes, qtd_sessoes, id) != NULL) {
                    printf("[ERRO] Ja existe uma sessao cadastrada com o ID %d!\n", id);
                    break;
                }

                printf("Titulo do Filme: ");
                ler_texto(titulo, sizeof(titulo));

                printf("Horario (ex: 19:30): ");
                ler_texto(horario, sizeof(horario));

                printf("Numero da Sala: ");
                scanf("%d", &sala);

                printf("Quantidade de Fileiras (Linhas): ");
                scanf("%d", &linhas);

                printf("Quantidade de Assentos por Fileira (Colunas): ");
                scanf("%d", &colunas);
                limpar_buffer();

                Sessao *nova = sessao_criar(id, titulo, horario, sala, linhas, colunas);
                if (nova != NULL) {
                    sessoes[qtd_sessoes++] = nova;
                    printf("[SUCESSO] Sessao '%s' criada com sucesso!\n", titulo);
                } else {
                    printf("[ERRO] Falha ao alocar memoria para a sessao.\n");
                }
                break;
            }

            case 2: {
                sessao_listar_todas(sessoes, qtd_sessoes);
                break;
            }

            case 3: {
                int id;
                printf("\nDigite o ID da sessao para ver o mapa: ");
                scanf("%d", &id);
                limpar_buffer();

                Sessao *s = sessao_buscar_por_id(sessoes, qtd_sessoes, id);
                if (s == NULL) {
                    printf("[ERRO] Nenhuma sessao encontrada com o ID %d.\n", id);
                } else {
                    sessao_exibir_mapa(s);
                }
                break;
            }

            case 4: {
                if (qtd_reservas >= MAX_RESERVAS) {
                    printf("[ERRO] Limite maximo de reservas atingido!\n");
                    break;
                }

                int id_sessao;
                printf("\n--- Realizar Reserva ---\n");
                printf("ID da Sessao desejada: ");
                scanf("%d", &id_sessao);
                limpar_buffer();

                Sessao *s = sessao_buscar_por_id(sessoes, qtd_sessoes, id_sessao);
                if (s == NULL) {
                    printf("[ERRO] Sessao nao encontrada!\n");
                    break;
                }

                sessao_exibir_mapa(s);

                int linha, coluna, codigo;
                char cliente[100];
                char cpf[20];

                printf("Escolha a Fileira (Linha iniciada em 1): ");
                scanf("%d", &linha);
                printf("Escolha o Assento (Coluna iniciada em 1): ");
                scanf("%d", &coluna);
                limpar_buffer();

                int idx_l = linha - 1;
                int idx_c = coluna - 1;

                if (!sessao_assento_livre(s, idx_l, idx_c)) {
                    printf("[ERRO] Assento [F%d C%d] ja ocupado ou invalido!\n", linha, coluna);
                    break;
                }

                printf("Codigo unico da Reserva (ex: 101): ");
                scanf("%d", &codigo);
                limpar_buffer();

                if (reserva_buscar_por_codigo(reservas, qtd_reservas, codigo) != NULL) {
                    printf("[ERRO] Ja existe uma reserva com o codigo %d!\n", codigo);
                    break;
                }

                printf("Nome do Cliente: ");
                ler_texto(cliente, sizeof(cliente));

                printf("CPF do Cliente: ");
                ler_texto(cpf, sizeof(cpf));

                Reserva *r = reserva_criar(codigo, id_sessao, cliente, cpf, idx_l, idx_c);
                if (r != NULL) {
                    sessao_marcar_assento(s, idx_l, idx_c, 1);
                    reservas[qtd_reservas++] = r;
                    printf("\n[SUCESSO] Reserva confirmada para %s no assento [F%d C%d]!\n", cliente, linha, coluna);
                } else {
                    printf("[ERRO] Falha ao alocar a reserva.\n");
                }
                break;
            }

            case 5: {
                int codigo;
                printf("\nDigite o codigo da reserva a cancelar: ");
                scanf("%d", &codigo);
                limpar_buffer();

                Reserva *r = reserva_buscar_por_codigo(reservas, qtd_reservas, codigo);
                if (r == NULL) {
                    printf("[ERRO] Nenhuma reserva localizada com o codigo %d.\n", codigo);
                    break;
                }

                Sessao *s = sessao_buscar_por_id(sessoes, qtd_sessoes, reserva_obter_id_sessao(r));
                if (reserva_cancelar(r, s)) {
                    printf("[SUCESSO] Reserva %d cancelada e cadeira liberada no mapa!\n", codigo);
                } else {
                    printf("[AVISO] Esta reserva ja se encontrava cancelada.\n");
                }
                break;
            }

            case 6: {
                reserva_listar_todas(reservas, qtd_reservas);
                break;
            }

            case 7: {
                int ok1 = persistencia_salvar_sessoes(ARQ_SESSOES, sessoes, qtd_sessoes);
                int ok2 = persistencia_salvar_reservas(ARQ_RESERVAS, reservas, qtd_reservas);

                if (ok1 && ok2) {
                    printf("[SUCESSO] Todos os dados foram gravados em disco!\n");
                } else {
                    printf("[ERRO] Falha ao persistir os dados no disco. Verifique se a pasta 'dados/' existe.\n");
                }
                break;
            }

            case 0: {
                printf("\nSalvando alteracoes no disco...\n");
                int ok1 = persistencia_salvar_sessoes(ARQ_SESSOES, sessoes, qtd_sessoes);
                int ok2 = persistencia_salvar_reservas(ARQ_RESERVAS, reservas, qtd_reservas);

                if (ok1 && ok2) {
                    printf("[SUCESSO] Dados salvos com sucesso.\n");
                } else {
                    printf("[AVISO] Nao foi possivel salvar tudo antes de sair. Verifique a pasta 'dados/'.\n");
                }
                break;
            }

            default:
                printf("[AVISO] Opcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    printf("Liberando memoria dinamica alocada...\n");
    for (int i = 0; i < qtd_reservas; i++) {
        reserva_deletar(reservas[i]);
    }
    for (int i = 0; i < qtd_sessoes; i++) {
        sessao_deletar(sessoes[i]);
    }

    printf("\nSistema encerrado com seguranca. Ate logo!\n");
    return 0;
}