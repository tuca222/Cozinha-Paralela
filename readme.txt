# Sistema de Cozinha paralela com Threads
Este projeto simula uma cozinha de restaurante utilizando programação paralela em C, onde múltiplos clientes fazem pedidos que são processados por múltiplos cozinheiros simultaneamente.

### Descrição
O sistema simula uma cozinha de restaurante onde:
- Clientes fazem pedidos que são adicionados a uma fila
- Cozinheiros processam os pedidos da fila de forma concorrente
- O sistema utiliza threads para simular o paralelismo entre clientes e cozinheiros
- Mecanismos de sincronização (mutex e variáveis de condição) são utilizados para garantir a consistência dos dados
- Foi adotado os padrões de projeto Produtor Consumir e Suspensão controlada para o desenvolvimento desse sistema

### Requisitos do Sistema
Para compilar e executar este programa, você precisará dos seguintes componentes:

1.  **Sistema Operacional:**
    -   Linux/Unix (recomendado devido ao uso da biblioteca `pthread`).

2.  **Compilador C:**
    -   GCC (GNU Compiler Collection) ou outro compilador C compatível com POSIX threads.

3.  **Biblioteca POSIX Threads (pthread):**
    -   Esta biblioteca é essencial para a criação e gerenciamento de threads. Geralmente, ela já vem instalada por padrão em sistemas Linux/Unix.

Dependendo do seu sistema operacional, você pode precisar instalar algumas dependências. Aqui estão os comandos para os sistemas Ubuntu/Debian:

Abra o terminal e execute:

sudo apt update
sudo apt install build-essential

O pacote build-essential inclui o GCC e outras ferramentas de compilação essenciais.

### Compilação
Para compilar o programa, abra o terminal e na pasta onde se encontra o arquivo cozinha_paralela.c execute:

gcc -o cozinha_paralela cozinha_paralela.c -pthread

### Execução
Para executar, no mesmo diretório onde foi compilado o programa, execute:

./cozinha paralela

O programa solicitará:
Número de clientes
Número de cozinheiros

### Funcionamento:
- Cada cliente é representado por uma thread que faz um pedido
- Cada cozinheiro é representado por uma thread que processa pedidos
- Os pedidos são gerenciados através de uma fila circular
- O programa termina quando todos os pedidos são processados
- Estruturas de Sincronização Utilizadas
- mutex_fila: Protege o acesso à fila de pedidos
- mutex_pedidos: Protege o contador de pedidos processados
- cond_novo_pedido: Sinaliza a chegada de novos pedidos

### Exemplo:
gcc -o cozinha_paralela cozinha_paralela.c -pthread
./cozinha paralela

Digite o número de clientes: 10
Digite o número de cozinheiros: 3

### Limitações
O programa foi desenvolvido para ambientes Unix/Linux
Não há tratamento para números negativos ou entradas inválidas
A capacidade máxima da fila é igual ao número de clientes
