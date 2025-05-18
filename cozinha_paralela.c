//Tabalho 1 - Programação Paralela
//Arthur Lorenzetti da Rosa - 19200621
//Jacqueline Correia Beber  - 19200634
//Vinicius Araujo           - 18205320


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Estrutura da fila de pedidos
typedef struct {
    int* pedidos;
    int inicio, fim, total;
    int capacidade;
} FilaPedidos;

// Variáveis globais
FilaPedidos fila;
pthread_mutex_t mutex_fila;
pthread_cond_t cond_novo_pedido;
pthread_mutex_t mutex_pedidos;
int pedidos_processados = 0;
int num_clientes;
int num_cozinheiros;
int todos_pedidos_feitos = 0;  // Nova variável para controle

// Função para inserir pedido na fila
void inserir_pedido(int pedido) {
    fila.pedidos[fila.fim] = pedido;
    fila.fim = (fila.fim + 1) % fila.capacidade;
    fila.total++;
}

// Função para retirar pedido da fila
int retirar_pedido() {
    int pedido = fila.pedidos[fila.inicio];
    fila.inicio = (fila.inicio + 1) % fila.capacidade;
    fila.total--;
    return pedido;
}

// Thread do cliente
void* cliente_thread(void* arg) {
    int id = *((int*)arg);
    free(arg);

    pthread_mutex_lock(&mutex_fila);
    inserir_pedido(id);
    printf("Cliente %d fez um pedido.\n", id);
    pthread_cond_broadcast(&cond_novo_pedido);
    pthread_mutex_unlock(&mutex_fila);

    pthread_exit(NULL);
}

// Thread do cozinheiro
void* cozinheiro_thread(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (1) {
        printf("Cozinheiro %d deu o lock na mutex_fila.\n", id);
        pthread_mutex_lock(&mutex_fila);

        // Enquanto não há pedidos, espera ou verifica se deve terminar
        while (fila.total == 0) {
            printf("Fila vazia no momento, Cozinheiro %d verificando se acabou o serviço (todos pedidos consumidos).\n", id);

            // Se todos os pedidos foram feitos e processados, termina
            if (todos_pedidos_feitos && pedidos_processados >= num_clientes) {
                pthread_mutex_unlock(&mutex_fila);
                printf("Cozinheiro %d finalizou o trabalho.\n", id);
                pthread_exit(NULL);
            }
            printf("Cozinheiro %d dormiu.\n", id);
            pthread_cond_wait(&cond_novo_pedido, &mutex_fila);
            printf("Cozinheiro %d acordou.\n", id);

        }

        // Processa o pedido
        int pedido = retirar_pedido();
        pthread_mutex_unlock(&mutex_fila);

        printf("Cozinheiro %d está preparando o pedido do cliente %d...\n", id, pedido);
        sleep(1); // Simula tempo de preparo

        pthread_mutex_lock(&mutex_pedidos);
        pedidos_processados++;
        printf("Cozinheiro %d finalizou o pedido do cliente %d. Pedidos processados: %d/%d\n", 
               id, pedido, pedidos_processados, num_clientes);
        pthread_mutex_unlock(&mutex_pedidos);

        // Verifica novamente se deve terminar após processar o pedido
        if (todos_pedidos_feitos && pedidos_processados >= num_clientes) {
            pthread_mutex_lock(&mutex_fila);
            pthread_cond_broadcast(&cond_novo_pedido);  // Acorda outros cozinheiros
            pthread_mutex_unlock(&mutex_fila);
            printf("Cozinheiro %d finalizou o trabalho.\n", id);
            pthread_exit(NULL);
        }
    }
}

int main() {
    // Obtém número de clientes e cozinheiros
    printf("Digite o número de clientes: ");
    scanf("%d", &num_clientes);
    printf("Digite o número de cozinheiros: ");
    scanf("%d", &num_cozinheiros);

    // Inicializa a fila
    fila.pedidos = malloc(sizeof(int) * num_clientes);
    fila.inicio = 0;
    fila.fim = 0;
    fila.total = 0;
    fila.capacidade = num_clientes;

    // Aloca memória para as threads
    pthread_t* clientes = malloc(sizeof(pthread_t) * num_clientes);
    pthread_t* cozinheiros = malloc(sizeof(pthread_t) * num_cozinheiros);

    // Inicializa mutexes e variável de condição
    pthread_mutex_init(&mutex_fila, NULL);
    pthread_mutex_init(&mutex_pedidos, NULL);
    pthread_cond_init(&cond_novo_pedido, NULL);

    printf("MUTEX e VAR_COND -- INICIADOS\n");

    // Cria threads dos cozinheiros
    for (int i = 0; i < num_cozinheiros; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&cozinheiros[i], NULL, cozinheiro_thread, id);
        printf("Cozinheiro %d CRIADO\n", *id);
        usleep(200000);
    }

    // Cria threads dos clientes
    for (int i = 0; i < num_clientes; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&clientes[i], NULL, cliente_thread, id);
        printf("Cliente %d CRIADO\n", *id);
        usleep(200000);
    }

    // Espera todos os clientes terminarem
    for (int i = 0; i < num_clientes; i++) {
        pthread_join(clientes[i], NULL);
    }

    // Marca que todos os pedidos foram feitos
    pthread_mutex_lock(&mutex_fila);
    todos_pedidos_feitos = 1;
    pthread_cond_broadcast(&cond_novo_pedido);
    pthread_mutex_unlock(&mutex_fila);

    // Espera todos os cozinheiros terminarem
    for (int i = 0; i < num_cozinheiros; i++) {
        pthread_join(cozinheiros[i], NULL);
    }

    // Limpa recursos
    pthread_mutex_destroy(&mutex_fila);
    pthread_mutex_destroy(&mutex_pedidos);
    pthread_cond_destroy(&cond_novo_pedido);
    free(fila.pedidos);
    free(clientes);
    free(cozinheiros);

    printf("Todos os pedidos foram processados. Fim do expediente.\n");
    return 0;
}