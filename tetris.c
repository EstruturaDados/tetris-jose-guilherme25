#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5  // Tamanho máximo da fila

// ------------------- ESTRUTURA DA PEÇA -------------------
typedef struct {
    char nome;
    int id;
} Peca;

// ------------------- PROTÓTIPOS ---------------------------
Peca gerarPeca(int id);
void exibirFila(Peca fila[], int inicio, int fim, int qtd);
void jogarPeca(Peca fila[], int *inicio, int *qtd);
void inserirPeca(Peca fila[], int *fim, int *qtd, int *idGlobal);

// ----------------------------------------------------------
int main() {
    Peca fila[TAM_FILA];
    int inicio = 0;
    int fim = 0;
    int qtd = 0;        // Quantidade atual na fila
    int idGlobal = 1;   // IDs únicos

    srand(time(NULL));

    // Inicializar com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        fila[fim] = gerarPeca(idGlobal++);
        fim = (fim + 1) % TAM_FILA;
        qtd++;
    }

    int opcao;

    while (1) {
        printf("\n===== TETRIS STACK =====\n");
        exibirFila(fila, inicio, fim, qtd);

        printf("\n1 - Jogar peça\n");
        printf("2 - Inserir nova peça\n");
        printf("3 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(fila, &inicio, &qtd);
                break;

            case 2:
                inserirPeca(fila, &fim, &qtd, &idGlobal);
                break;

            case 3:
                printf("Encerrando...\n");
                return 0;

            default:
                printf("Opção inválida!\n");
        }
    }
}

// ------------------- GERA UMA NOVA PEÇA -------------------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ------------------- EXIBIR FILA ---------------------------
void exibirFila(Peca fila[], int inicio, int fim, int qtd) {
    printf("\n--- Estado da Fila (%d peças) ---\n", qtd);

    if (qtd == 0) {
        printf("Fila vazia!\n");
        return;
    }

    int pos = inicio;
    for (int i = 0; i < qtd; i++) {
        printf("Peça '%c' (ID %d)\n",
               fila[pos].nome,
               fila[pos].id);

        pos = (pos + 1) % TAM_FILA;
    }
}

// ------------------- REMOVER PEÇA (DEQUEUE) ----------------
void jogarPeca(Peca fila[], int *inicio, int *qtd) {
    if (*qtd == 0) {
        printf("\nNenhuma peça disponível para jogar!\n");
        return;
    }

    printf("\nPeça jogada: '%c' (ID %d)\n",
           fila[*inicio].nome,
           fila[*inicio].id);

    *inicio = (*inicio + 1) % TAM_FILA;
    (*qtd)--;
}

// ------------------- INSERIR PEÇA (ENQUEUE) ----------------
void inserirPeca(Peca fila[], int *fim, int *qtd, int *idGlobal) {
    if (*qtd == TAM_FILA) {
        printf("\nA fila está cheia! Não é possível inserir mais peças.\n");
        return;
    }

    fila[*fim] = gerarPeca((*idGlobal)++);
    *fim = (*fim + 1) % TAM_FILA;
    (*qtd)++;

    printf("\nNova peça inserida com sucesso!\n");
}
