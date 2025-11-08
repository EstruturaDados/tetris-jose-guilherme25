#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5       // Capacidade fixa da fila de peças futuras
#define TAM_PILHA 3      // Capacidade fixa da pilha de reserva

// Representa uma peça do Tetris
typedef struct {
    char nome;   // 'I', 'O', 'T', 'L'
    int id;      // identificador único (ordem de criação)
} Peca;

/* --- Protótipos --- */
Peca gerarPeca(int id);
void exibirEstado(Peca fila[], int inicio, int fim, int qtdFila, Peca pilha[], int topoPilha);
void inicializarFila(Peca fila[], int *fim, int *qtdFila, int *idGlobal);
int filaVazia(int qtd);
int filaCheia(int qtd);
int pilhaVazia(int topo);
int pilhaCheia(int topo);
void jogarPeca(Peca fila[], int *inicio, int *qtdFila);
int reservarPeca(Peca fila[], int *inicio, int *qtdFila, Peca pilha[], int *topoPilha);
int usarPecaReservada(Peca pilha[], int *topoPilha);

/* --- Função main --- */
int main(void) {
    Peca fila[TAM_FILA];
    Peca pilha[TAM_PILHA];

    int inicio = 0;   // índice do início da fila (elemento a ser jogado/reservado)
    int fim = 0;      // índice onde será inserida a próxima peça
    int qtdFila = 0;  // quantidade atual na fila
    int topoPilha = -1; // índice do topo da pilha (-1 = vazia)
    int idGlobal = 1; // contador de IDs únicos

    int opcao;

    srand((unsigned int)time(NULL));

    // Inicializa fila com TAM_FILA peças
    inicializarFila(fila, &fim, &qtdFila, &idGlobal);

    do {
        printf("\n===== TETRIS STACK =====\n");
        exibirEstado(fila, inicio, fim, qtdFila, pilha, topoPilha);

        printf("\nMenu:\n");
        printf("1 - Jogar peça (remover da fila)\n");
        printf("2 - Reservar peça (mover da fila para a pilha)\n");
        printf("3 - Usar peça da reserva (remover do topo da pilha)\n");
        printf("4 - Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            // entrada inválida: limpa buffer e continua
            while (getchar() != '\n');
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                // Jogar peça: remove da frente da fila (se houver)
                if (filaVazia(qtdFila)) {
                    printf("\nA fila está vazia; nada para jogar.\n");
                } else {
                    jogarPeca(fila, &inicio, &qtdFila);
                    // Depois de remover da fila, geramos e enfileiramos nova peça para manter a fila cheia
                    if (!filaCheia(qtdFila)) {
                        fila[fim] = gerarPeca(idGlobal++);
                        fim = (fim + 1) % TAM_FILA;
                        qtdFila++;
                        printf("Nova peça gerada e adicionada ao final da fila para repor (fila mantida).\n");
                    }
                }
                break;

            case 2:
                // Reservar peça: move frente da fila para topo da pilha (se houver espaço)
                if (filaVazia(qtdFila)) {
                    printf("\nA fila está vazia; nada para reservar.\n");
                } else if (pilhaCheia(topoPilha)) {
                    printf("\nA pilha de reserva está cheia; não é possível reservar.\n");
                } else {
                    if (reservarPeca(fila, &inicio, &qtdFila, pilha, &topoPilha)) {
                        // Ao mover da fila para a pilha, a fila perdeu um elemento => gerar e enfileirar uma nova peça
                        if (!filaCheia(qtdFila)) {
                            fila[fim] = gerarPeca(idGlobal++);
                            fim = (fim + 1) % TAM_FILA;
                            qtdFila++;
                            printf("Peça gerada e adicionada ao final da fila para repor (fila mantida).\n");
                        }
                    }
                }
                break;

            case 3:
                // Usar peça da reserva: remove do topo da pilha
                if (pilhaVazia(topoPilha)) {
                    printf("\nA pilha de reserva está vazia; nada para usar.\n");
                } else {
                    Peca usada = pilha[topoPilha--];
                    printf("\nPeça usada da reserva: '%c' (ID %d)\n", usada.nome, usada.id);

                    // Observação: como usar peça da reserva **não remove** nada da fila,
                    // normalmente a fila segue cheia. Ainda assim, caso haja espaço por algum motivo,
                    // tentamos enfileirar uma nova peça para manter a fila o mais cheia possível.
                    if (!filaCheia(qtdFila)) {
                        fila[fim] = gerarPeca(idGlobal++);
                        fim = (fim + 1) % TAM_FILA;
                        qtdFila++;
                        printf("Fila tinha espaço; nova peça gerada e adicionada ao final.\n");
                    } else {
                        // fila já está cheia — não fazemos remoção automática para abrir espaço.
                        printf("Fila já está cheia; nenhuma peça adicionada após usar a reserva.\n");
                    }
                }
                break;

            case 4:
                printf("Encerrando o programa. Até logo!\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (opcao != 4);

    return 0;
}

/* --- Implementação das funções --- */
// Gera uma peça com tipo aleatório e id informado
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Exibe a fila (sem índice) e a pilha (do topo para base), com separação visual
void exibirEstado(Peca fila[], int inicio, int fim, int qtdFila, Peca pilha[], int topoPilha) {
    printf("\n--- Fila de Peças Futuras (próximas) ---\n");
    if (qtdFila == 0) {
        printf("Fila vazia!\n");
    } else {
        int pos = inicio;
        for (int i = 0; i < qtdFila; i++) {
            printf("Peça '%c' (ID %d)\n", fila[pos].nome, fila[pos].id);
            pos = (pos + 1) % TAM_FILA;
        }
    }

    printf("\n--- Pilha de Reserva (topo -> base) ---\n");
    if (pilhaVazia(topoPilha)) {
        printf("Pilha vazia!\n");
    } else {
        for (int i = topoPilha; i >= 0; i--) {
            printf("Peça '%c' (ID %d)\n", pilha[i].nome, pilha[i].id);
        }
    }
}

// Inicializa a fila com TAM_FILA peças geradas automaticamente
void inicializarFila(Peca fila[], int *fim, int *qtdFila, int *idGlobal) {
    for (int i = 0; i < TAM_FILA; i++) {
        fila[*fim] = gerarPeca((*idGlobal)++);
        *fim = (*fim + 1) % TAM_FILA;
        (*qtdFila)++;
    }
}

// Verificadores de vazio/cheio para fila e pilha
int filaVazia(int qtd) {
    return qtd == 0;
}
int filaCheia(int qtd) {
    return qtd == TAM_FILA;
}
int pilhaVazia(int topo) {
    return topo == -1;
}
int pilhaCheia(int topo) {
    return topo == (TAM_PILHA - 1);
}

// Remove (dequeue) a peça da frente da fila e informa qual peça foi jogada
void jogarPeca(Peca fila[], int *inicio, int *qtdFila) {
    if (filaVazia(*qtdFila)) {
        printf("\nA fila está vazia, nada para jogar.\n");
        return;
    }

    Peca jogada = fila[*inicio];
    printf("\nPeça jogada: '%c' (ID %d)\n", jogada.nome, jogada.id);

    *inicio = (*inicio + 1) % TAM_FILA;
    (*qtdFila)--;
}

// Move a peça da frente da fila para o topo da pilha (se possível).
// Retorna 1 se sucesso, 0 caso contrário.
int reservarPeca(Peca fila[], int *inicio, int *qtdFila, Peca pilha[], int *topoPilha) {
    if (filaVazia(*qtdFila)) {
        return 0;
    }
    if (pilhaCheia(*topoPilha)) {
        return 0;
    }

    // remover o elemento da frente da fila
    Peca movida = fila[*inicio];
    *inicio = (*inicio + 1) % TAM_FILA;
    (*qtdFila)--;

    // empilhar no topo da pilha
    (*topoPilha)++;
    pilha[*topoPilha] = movida;

    printf("\nPeça reservada: '%c' (ID %d) -> empilhada na reserva.\n", movida.nome, movida.id);
    return 1;
}

// Usa (pop) a peça do topo da pilha. Retorna 1 se houve remoção, 0 se pilha vazia.
int usarPecaReservada(Peca pilha[], int *topoPilha) {
    if (pilhaVazia(*topoPilha)) return 0;
    // A lógica de remoção e uso está no main, aqui apenas placeholder caso queira modularizar
    (*topoPilha)--;
    return 1;
}
