#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5    // Capacidade da fila circular de peças futuras
#define TAM_PILHA 3   // Capacidade da pilha de reserva

/* Representação de uma peça */
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

/* Protótipos */
Peca gerarPeca(int id);
void inicializarFila(Peca fila[], int *fim, int *qtdFila, int *idGlobal);
void exibirEstado(Peca fila[], int inicio, int fim, int qtdFila, Peca pilha[], int topoPilha);
int filaVazia(int qtd);
int filaCheia(int qtd);
int pilhaVazia(int topo);
int pilhaCheia(int topo);
void jogarPeca(Peca fila[], int *inicio, int *qtdFila);
int reservarPeca(Peca fila[], int *inicio, int *qtdFila, Peca pilha[], int *topoPilha);
int usarPecaReservada(Peca pilha[], int *topoPilha);
int trocarAtual(Peca fila[], int inicio, Peca pilha[], int topoPilha);
int trocaMultipla(Peca fila[], int inicio, int qtdFila, Peca pilha[], int topoPilha);

/*--- main ---*/
int main(void) {
    Peca fila[TAM_FILA];
    Peca pilha[TAM_PILHA];

    int inicio = 0;    // índice do início da fila (elemento da frente)
    int fim = 0;       // índice de inserção no final da fila
    int qtdFila = 0;   // quantidade atual na fila
    int topoPilha = -1; // topo da pilha (-1 = vazia)
    int idGlobal = 1;  // contador de IDs únicos

    int opcao;

    srand((unsigned int)time(NULL));

    // Inicializa fila com TAM_FILA peças
    inicializarFila(fila, &fim, &qtdFila, &idGlobal);

    do {
        printf("\n===== TETRIS STACK - Gerenciador Avançado =====\n");
        exibirEstado(fila, inicio, fim, qtdFila, pilha, topoPilha);

        printf("\nMenu:\n");
        printf("1 - Jogar peça (remover da fila)\n");
        printf("2 - Reservar peça (mover da fila para a pilha)\n");
        printf("3 - Usar peça da reserva (remover do topo da pilha)\n");
        printf("4 - Trocar peça atual (frente da fila <-> topo da pilha)\n");
        printf("5 - Troca múltipla (3 primeiras da fila <-> 3 da pilha)\n");
        printf("6 - Visualizar estado (mostrar fila e pilha)\n");
        printf("7 - Sair\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            // limpar entrada inválida
            while (getchar() != '\n');
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                // Jogar peça (remove da frente da fila)
                if (filaVazia(qtdFila)) {
                    printf("\nA fila está vazia; nada para jogar.\n");
                } else {
                    jogarPeca(fila, &inicio, &qtdFila);
                    // gerar nova peça para repor a fila (quando possível)
                    if (!filaCheia(qtdFila)) {
                        fila[fim] = gerarPeca(idGlobal++);
                        fim = (fim + 1) % TAM_FILA;
                        qtdFila++;
                        printf("Peça gerada e adicionada ao final da fila para repor.\n");
                    }
                }
                break;

            case 2:
                // Reservar peça (move frente -> topo da pilha)
                if (filaVazia(qtdFila)) {
                    printf("\nA fila está vazia; nada para reservar.\n");
                } else if (pilhaCheia(topoPilha)) {
                    printf("\nA pilha de reserva está cheia; não é possível reservar.\n");
                } else {
                    if (reservarPeca(fila, &inicio, &qtdFila, pilha, &topoPilha)) {
                        // gerar nova peça para repor a fila
                        if (!filaCheia(qtdFila)) {
                            fila[fim] = gerarPeca(idGlobal++);
                            fim = (fim + 1) % TAM_FILA;
                            qtdFila++;
                            printf("Peça gerada e adicionada ao final da fila para repor.\n");
                        }
                    }
                }
                break;

            case 3:
                // Usar peça reservada (pop)
                if (pilhaVazia(topoPilha)) {
                    printf("\nA pilha de reserva está vazia; nada para usar.\n");
                } else {
                    Peca usada = pilha[topoPilha--];
                    printf("\nPeça usada da reserva: '%c' (ID %d)\n", usada.nome, usada.id);
                    // Não geramos aqui automaticamente (a fila normalmente segue cheia).
                }
                break;

            case 4:
                // Trocar peça atual (frente da fila <-> topo da pilha)
                if (trocarAtual(fila, inicio, pilha, topoPilha)) {
                    printf("\nTroca realizada entre frente da fila e topo da pilha.\n");
                } else {
                    printf("\nNão foi possível realizar a troca (fila ou pilha vazia).\n");
                }
                break;

            case 5:
                // Troca múltipla (3 primeiras da fila <-> 3 da pilha)
                if (trocaMultipla(fila, inicio, qtdFila, pilha, topoPilha)) {
                    printf("\nTroca múltipla (3 peças) realizada com sucesso.\n");
                } else {
                    printf("\nTroca múltipla não realizada. Verifique se ambas têm pelo menos 3 peças.\n");
                }
                break;

            case 6:
                // Apenas exibe (já exibimos no topo do loop, mas deixar opção)
                printf("\nVisualização solicitada.\n");
                break;

            case 7:
                printf("Encerrando o programa. Até logo!\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 7);

    return 0;
}

/*--- Implementação das funções ---*/
// Gera uma peça com tipo aleatório e id informado
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Inicializa a fila (circular) com TAM_FILA peças geradas automaticamente
void inicializarFila(Peca fila[], int *fim, int *qtdFila, int *idGlobal) {
    for (int i = 0; i < TAM_FILA; i++) {
        fila[*fim] = gerarPeca((*idGlobal)++);
        *fim = (*fim + 1) % TAM_FILA;
        (*qtdFila)++;
    }
}

// Exibe a fila e a pilha de forma clara (fila: frente -> ... ; pilha: topo -> base)
// Não exibe índices, apenas nome e ID
void exibirEstado(Peca fila[], int inicio, int fim, int qtdFila, Peca pilha[], int topoPilha) {
    printf("\n--- Fila de Peças Futuras (frente -> fim) ---\n");
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
    if (topoPilha == -1) {
        printf("Pilha vazia!\n");
    } else {
        for (int i = topoPilha; i >= 0; i--) {
            printf("Peça '%c' (ID %d)\n", pilha[i].nome, pilha[i].id);
        }
    }
}

// Verificadores de estado
int filaVazia(int qtd) { return qtd == 0; }
int filaCheia(int qtd) { return qtd == TAM_FILA; }
int pilhaVazia(int topo) { return topo == -1; }
int pilhaCheia(int topo) { return topo == (TAM_PILHA - 1); }

// Remove (dequeue) a peça da frente da fila e informa qual peça foi jogada
void jogarPeca(Peca fila[], int *inicio, int *qtdFila) {
    if (filaVazia(*qtdFila)) {
        printf("\nA fila está vazia; nada a jogar.\n");
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
    if (filaVazia(*qtdFila)) return 0;
    if (pilhaCheia(*topoPilha)) return 0;

    // Retira da frente da fila
    Peca movida = fila[*inicio];
    *inicio = (*inicio + 1) % TAM_FILA;
    (*qtdFila)--;

    // Empilha no topo
    (*topoPilha)++;
    pilha[*topoPilha] = movida;

    printf("\nPeça reservada: '%c' (ID %d)\n", movida.nome, movida.id);
    return 1;
}

// Usa (pop) a peça do topo da pilha. Retorna 1 se houve remoção, 0 se pilha vazia.
int usarPecaReservada(Peca pilha[], int *topoPilha) {
    if (pilhaVazia(*topoPilha)) return 0;
    (*topoPilha)--;
    return 1;
}

// Troca a peça da frente da fila com o topo da pilha.
// Retorna 1 se a troca aconteceu, 0 se não foi possível (fila ou pilha vazia).
int trocarAtual(Peca fila[], int inicio, Peca pilha[], int topoPilha) {
    if (filaVazia( (fila == NULL) ? 0 : 1) ) {
        // nota: checagem lógica não usada aqui; vamos checar explicitamente fora antes de chamar
    }
    if (topoPilha == -1) return 0; // pilha vazia
    // A função pressupõe que a fila tem ao menos 1 elemento no índice 'inicio'.
    // Quem chama deve garantir isso (o menu faz as checagens).
    // Para evitar acessar lixo, só faz troca se aparentemente a fila não está vazia.
    // No contexto do main nós sempre inicializamos a fila cheia, então segue:
    Peca tmp = fila[inicio];
    fila[inicio] = pilha[topoPilha];
    pilha[topoPilha] = tmp;
    return 1;
}

// Troca múltipla: alterna as 3 primeiras peças da fila (frente) com as 3 peças do topo da pilha.
// Condição: qtdFila >= 3 e topoPilha >= 2 (ou seja, pilha possui pelo menos 3 elementos).
// Retorna 1 se sucesso, 0 caso contrário.
int trocaMultipla(Peca fila[], int inicio, int qtdFila, Peca pilha[], int topoPilha) {
    if (qtdFila < 3) return 0;
    if (topoPilha < 2) return 0; // menos de 3 peças na pilha

    // Troca: fila[frente+i] <-> pilha[topo - i], para preservar ordem relativa
    for (int i = 0; i < 3; i++) {
        int posFila = (inicio + i) % TAM_FILA;
        int posPilha = topoPilha - i; // top, top-1, top-2
        Peca tmp = fila[posFila];
        fila[posFila] = pilha[posPilha];
        pilha[posPilha] = tmp;
    }
    return 1;
}
