# Guia de Estudos Mini_serv (Versão Struct)

Este guia detalha a implementação do [pt/struct/mini_serv.c](pt/struct/mini_serv.c) para o exame Rank 06. Ele incorpora os requisitos estritos do [subject.en.txt](../subject/subject.en.txt) e utiliza o [main.c](../subject/main.c) tipicamente fornecido.

> [!TIP]
> Este guia foca na **abordagem com struct** (`pt/struct/mini_serv.c`). A abordagem com arrays (`pt/array/mini_serv.c`) também é coberta como referência, mas a versão com struct é recomendada pela melhor organização do código. A lógica é **idêntica** — só o acesso aos dados muda.

---

## 1. Contexto Teórico e Fundamentos

O desenvolvimento de aplicações de rede no Linux baseia-se na API de Sockets. O modelo UNIX dita que "quase tudo é um arquivo". Portanto, para realizar I/O em sockets, utilizamos chamadas de sistema parecidas com as de arquivos normais (como `read` e `write`). Cada conexão é representada por um descritor de arquivo (fd), que é apenas um pequeno número inteiro não negativo.

**O Desafio da Concorrência:**
Um servidor de chat precisa lidar com múltiplos clientes simultaneamente. O uso de chamadas bloqueantes (como um `recv` que fica esperando dados de um único cliente) impediria o servidor de atender outros usuários. Para resolver isso sem o uso de threads (que são proibidas no contexto do mini_serv), utilizamos a técnica de **multiplexação de I/O** com a função `select()`.

---

## 2. Regras e Requisitos do Exame (Crucial)

### Funções Permitidas
`write`, `close`, `select`, `socket`, `accept`, `listen`, `send`, `recv`, `bind`, `strstr`, `malloc`, `realloc`, `free`, `calloc`, `bzero`, `atoi`, `sprintf`, `strlen`, `exit`, `strcpy`, `strcat`, `memset`.
> **AVISO:** `printf` é **PROIBIDO**. Use `write` para tudo (ou `send` para sockets). Também **PROIBIDO usar `#define`** como diretiva de pré-processador.

### Mensagens de Saída Estritas
O testador do exame verifica estas strings exatas. Um erro de digitação = reprovação.
1.  **Erro de Argumento**: `Wrong number of arguments\n` → stderr, exit 1
2.  **Erro de Sistema/Malloc**: `Fatal error\n` → stderr, exit 1
3.  **Chegada de Cliente**: `server: client %d just arrived\n`
4.  **Saída de Cliente**: `server: client %d just left\n`
5.  **Prefixo de Mensagem**: `client %d: ` → prefixado em **cada linha** da mensagem

> [!CAUTION]
> O formato da mensagem é `client %d: ` (com um espaço após os dois-pontos). Um único espaço faltando ou newline errado falhará no diff.

### Rede e Comportamento
-   **IP**: Deve ligar (bind) em `127.0.0.1` (hardcoded como `htonl(2130706433)`).
-   **Porta**: Passada como o primeiro argumento (`argv[1]`).
-   **Não-bloqueante**: O programa deve ser não-bloqueante (gerenciado via `select`), mas **NÃO** desconecte clientes "preguiçosos".
-   **IDs**: IDs de clientes começam em 0 e incrementam indefinidamente (nunca reutilizados).
-   **Velocidade**: Envie mensagens o mais rápido possível — não faça buffer desnecessário.

---

## 3. Análise do Código Fornecido ([main.c](../subject/main.c))

O exame fornece um arquivo `main.c` com uma estrutura básica. Você deve adaptá-lo seguindo as regras e a lógica de multiplexação.

### O que MANTER do main.c:
1. **[extract_message](../subject/main.c#L8-L33)**: Essencial para lidar com o TCP (que é um stream). Separa o buffer em linhas completas terminadas por `\n`.
2. **[str_join](../subject/main.c#L35-L53)**: Usada para acumular dados parciais. **Atenção:** Esta função dá `free()` no primeiro argumento passado automaticamente!
3. **Boilerplate de Socket**: A criação do socket, preenchimento do `sockaddr_in` (IP 127.0.0.1 e porta), `bind()` e `listen()`.

### O que ALTERAR ou REMOVER:
1. **Remover o `printf`**: É proibido. Substitua por `write` (no stderr para erros) ou `send` para a rede.
2. **Substituir o `accept()` único**: O código original aceita apenas um cliente. Substitua-o por um `while(1)` contendo o loop de eventos do `select()`.
3. **Implementar o Gerenciamento de Clientes**: Você precisará gerenciar buffers e IDs indexados pelo `fd` do cliente usando uma struct (como `t_client clients[FD_SETSIZE]`).

---

## 4. Conceitos Principais

### Multiplexação com `select()`
O desafio principal é lidar com múltiplos clientes sem threads. `select()` permite que o programa "observe" múltiplos descritores de arquivo (sockets) de uma vez e acorde apenas quando um deles estiver pronto.

| Macro/Função | Propósito |
|---|---|
| `FD_ZERO(&set)` | Limpa todo o conjunto |
| `FD_SET(fd, &set)` | Adiciona `fd` ao conjunto |
| `FD_CLR(fd, &set)` | Remove `fd` do conjunto |
| `FD_ISSET(fd, &set)` | Verifica se `fd` está no conjunto (usado **após** o retorno do `select`) |
| `select(maxfd+1, &read, &write, NULL, NULL)` | Bloqueia até que ocorra atividade |

> [!WARNING]
> **Regra de Ouro: O `select()` é destrutivo!**
> Ele modifica os conjuntos que você passa para ele para indicar quem está pronto. Você deve **sempre** manter um conjunto mestre (`master`) intacto e copiar seus valores para o conjunto de trabalho (`readfds`) no **início** de cada iteração do loop principal.

### Gerenciamento de Buffer
TCP é um protocolo de **fluxo** (stream), não de pacote:
- Você pode receber uma mensagem **parcial** (ex: `"Ol"`)
- Você pode receber **múltiplas** mensagens de uma vez (ex: `"Ola\nMundo\n"`)
- **Solução**: Acumular dados recebidos em um buffer por cliente e extrair linhas completas terminadas em `\n` usando [extract_message](../subject/main.c#L8-L33).

### Correções de Tipos POSIX (Em relação ao main.c)
O `main.c` fornecido tem algumas imprecisões de tipos que causam avisos de compilação ou vulnerabilidades. Nossa solução corrige isso:

1. **`accept` com parâmetros `NULL` (Simplificação Total)**:
   O `main.c` original cria variáveis (`int len` e `struct sockaddr_in cli`) para receber o IP e a porta de quem conectou via `accept()`. Mas o nosso chat não precisa dessa informação! Segundo a página `man 2 accept`, se você não precisa do endereço do cliente, você pode passar ponteiros `NULL`. Nós substituímos toda essa alocação simplesmente por `accept(serverfd, NULL, NULL)`.
   **Por que é melhor?** Você poupa tempo no exame (menos digitação), elimina variáveis não utilizadas e, de quebra, escapa do problema de ter que corrigir o tipo `int len` para `socklen_t` (que causa avisos de compilação em 64-bits). A título de curiosidade, a forma purista/educacional caso você *precisasse* do IP seria declarar `struct sockaddr addr; socklen_t lensock = sizeof(addr);` e passar os endereços.
2. **Retorno de `recv` em `ssize_t`**:
   O `recv()` retorna o número de bytes lidos ou `-1` em erro. O tipo `ssize_t` (Signed Size) é projetado exatamente para isso, pois acomoda o limite máximo de memória da arquitetura sem risco de *overflow* (diferente de um `int` comum).

> **Onde ler mais sobre as Structs de Endereço:**
> Os manuais do `accept` e `recv` não detalham a fundo essas estruturas, pois elas dependem da camada de rede (IPv4, IPv6, Unix Sockets, etc). Para entender a hierarquia delas, consulte:
> * **`man 7 socket`**: Explica a API geral e a struct genérica `sockaddr`.
> * **`man 7 ip`**: Explica a implementação IPv4 específica e a `sockaddr_in`.
> * **Beej's Guide to Network Programming:** A seção *3. IP Addresses, structs, and Data Munging* é a melhor explicação didática existente sobre como o C lida com essa "salada" de structs herdada dos anos 80.
> * **The Linux Programming Interface (TLPI):** Por Michael Kerrisk. É o livro definitivo sobre a interface de sistema do Linux. Os capítulos 56 a 61 mergulham fundo nos fundamentos teóricos e práticos de Sockets e Multiplexação de I/O.

---

## 5. Estruturas de Dados

### Abordagem com Struct (Principal — `pt/struct/mini_serv.c`)
Usa uma struct para coesão dos dados:
```c
typedef struct s_client {
    int   id;    // ID sequencial do cliente
    char  *msg;  // Buffer de mensagem acumulada
} t_client;

t_client clients[FD_SETSIZE]; // Indexado pelo fd
```
**Acesso**: `clients[fd].id` e `clients[fd].msg`

**Vantagens:**
- Dados de um cliente ficam agrupados
- Mais fácil adicionar campos futuros (ex: apelido)
- Código mais limpo e auto-documentável

### Abordagem com Arrays (Alternativa — `pt/array/mini_serv.c`)
Usa arrays paralelos:
```c
int   ids[65536];          // fd → ID do cliente
char  *client_msg[65536];  // fd → buffer de mensagem
```
**Acesso**: `ids[fd]` e `client_msg[fd]`

**Vantagens:**
- Levemente menos digitação para um exame de velocidade
- Conceitualmente mais simples

> Ambas as abordagens usam a mesma lógica e ambas possuem o guard `if (newfd >= FD_SETSIZE)` que previne comportamento indefinido no `FD_SET`. A versão struct dimensiona seu array para `FD_SETSIZE` (tipicamente 1024) já que nenhum fd acima disso será armazenado, enquanto a versão array usa 65536 como segurança extra no acesso ao array — mas a verdadeira proteção contra UB vem do guard, não do tamanho do array. Ambas funcionam para o exame.

### Variáveis Globais (Comuns a Ambas)
```c
char   buffer_msg[128];  // Buffer auxiliar para sprintf (mensagens do servidor)
int    serverfd = -1;    // fd do socket de escuta
int    maxfds = 0;       // Maior fd rastreado (para o 1º argumento do select)
int    current_id = -1;  // Contador; começa em -1 para que o 1º cliente receba ID 0 via ++

fd_set master;    // Conjunto persistente de TODOS os fds ativos
fd_set readfds;   // Cópia temporária para select() — quem tem dados para ler?
```

---

## 6. Análise do Código (Versão Struct)

### Tratamento de Erros — [print_error](pt/struct/mini_serv.c#L161-L168)
```c
void print_error(char *msg)
{
    if (msg)
        write(2, msg, strlen(msg));
    else
        write(2, "Fatal error\n", 12);
    exit(1);
}
```
- Dupla função: passe uma mensagem específica (`"Wrong number of arguments\n"`) ou `NULL` para o erro genérico.
- Sempre escreve em **stderr** (fd 2) e sai com código **1**.

### Broadcasting — [sendAll](pt/struct/mini_serv.c#L180-L188)
```c
void sendAll(int sender, char *msg)
{
    int len = strlen(msg);
    for (int fd = 0; fd <= maxfds; fd++)
    {
        if (FD_ISSET(fd, &master) && fd != serverfd && fd != sender)
            send(fd, msg, len, 0);
    }
}
```

> [!TIP]
> **Por que checamos o `&master` e não o `&writefds`?**
> Na vida real, chamar `send()` se o buffer do cliente estiver cheio fará o servidor travar (bloquear). Usar o `writefds` no `select()` previne isso.
> Porém, no exame `mini_serv`, os dados são curtos, executados localmente e os "clientes" da Moulinette leem tudo instantaneamente. O buffer nunca enche.
> Como o `send()` nunca vai bloquear, remover a burocracia do `writefds` corta várias linhas de código do seu arquivo!

Três condições filtram quem recebe a mensagem:
1. **`FD_ISSET(fd, &master)`** — Confirma que o cliente existe na rede e está validado pelo servidor
2. **`fd != serverfd`** — Não enviar para o socket de escuta
3. **`fd != sender`** — Não ecoar de volta para o remetente

> [!NOTE]
> **Por que novos clientes não recebem sua própria mensagem "just arrived":**
> A função `sendAll` ignora o remetente através da condição `fd != sender`. Como passamos o `fd` do novo cliente como remetente da mensagem de "just arrived", ele é filtrado naturalmente e não a recebe.

### Processamento de Mensagens — [processMessages](pt/struct/mini_serv.c#L198-L220)
```c
void processMessages(int sender)
{
    char *msg;
    sprintf(buffer_msg, "client %d: ", clients[sender].id);

    while (extract_message(&clients[sender].msg, &msg) == 1)
    {
        sendAll(sender, buffer_msg);  // Envia prefixo "client X: "
        sendAll(sender, msg);         // Envia a linha (inclui '\n')
        free(msg);                    // Libera memória do extract_message
    }
}
```
Pontos-chave:
- `sprintf` monta o prefixo **uma vez** antes do loop (o ID não muda)
- Duas chamadas `sendAll` separadas: uma para o prefixo, outra para o conteúdo da linha
- O loop `while` trata o caso de múltiplos `\n` em um recv

### Inicialização do Servidor — [init_server](pt/struct/mini_serv.c#L233-L266)
Extraído do boilerplate do main.c — `socket()` → `bzero()` → configura endereço → `bind()` → `listen()`. Únicas diferenças do main.c:
- Porta vem de `argv[1]` em vez de hardcoded `8081`
- Erros chamam `print_error(NULL)` em vez de `printf`

### Aceitando Clientes — [accept_client](pt/struct/mini_serv.c#L273-L310)
```c
void accept_client()
{
    int newfd = accept(serverfd, NULL, NULL);
    if (newfd == -1)
        return ; // O main.c original dá exit(), mas um erro no accept() não deve encerrar o servidor.

    if (newfd >= FD_SETSIZE)       // Segurança: select() não suporta fds >= 1024
    {
        close(newfd);
        return ;
    }

    FD_SET(newfd, &master);          // Adiciona à lista de monitoramento
    clients[newfd].id = ++current_id; // Atribui ID sequencial
    clients[newfd].msg = NULL;        // Inicializa buffer vazio
    if (newfd > maxfds)              // Atualiza range do select()
        maxfds = newfd;

    sprintf(buffer_msg, "server: client %d just arrived\n", clients[newfd].id);
    sendAll(newfd, buffer_msg);
}
```

> [!WARNING]
> A verificação de `FD_SETSIZE` é importante! `FD_SET` com um fd ≥ `FD_SETSIZE` causa **comportamento indefinido**. No exame você pode omiti-la se estiver sem tempo, mas é boa prática.

### Tratando Dados do Cliente — [handle_client](pt/struct/mini_serv.c#L319-L359)
```c
void handle_client(int fd)
{
    char buffer[4096];
    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)                          // Desconexão ou erro
    {
        sprintf(buffer_msg, "server: client %d just left\n", clients[fd].id);
        sendAll(fd, buffer_msg);
        free(clients[fd].msg);                   // free(NULL) é seguro — não precisa de guard
        clients[fd].msg = NULL;                  // Previne lixo se o fd for reciclado
        FD_CLR(fd, &master);
        close(fd);
        return;
    }
    buffer[bytes] = '\0';                    // Terminador nulo para str_join
    clients[fd].msg = str_join(clients[fd].msg, buffer);
    processMessages(fd);
}
```

### Loop Principal — [run_server](pt/struct/mini_serv.c#L367-L421)
```c
void run_server()
{
    maxfds = serverfd;
    FD_ZERO(&master);
    FD_SET(serverfd, &master);

    while (1)
    {
        readfds = master;       // Copia! select() vai modificar

        int ret = select(maxfds + 1, &readfds, NULL, NULL, NULL);
        if (ret == -1)          // Fatal — não dá para recuperar de select quebrado
            print_error(NULL);  // Obs: O correto seria fechar todos os FDs antes de sair. Omitimos esse
                                // loop de limpeza para economizar tempo no exame (não é testado).

        for (int fd = 0; fd <= maxfds; fd++)
        {
            if (!FD_ISSET(fd, &readfds))   // Sem dados pendentes → pula
                continue ;

            if (fd == serverfd)
                accept_client();
            else
                handle_client(fd);
        }
    }
}
```

> [!IMPORTANT]
> **Por que `select() == -1` é fatal (e não apenas `continue`):**
> Sem um `select()` bem-sucedido, os conjunto `readfds` contém lixo. Um `continue` voltaria ao loop, copiaria `master` novamente, mas se o problema persistir (ex: fd inválido no master), faria um busy-loop infinito. O enunciado diz que erros de chamada de sistema = erro fatal.

> [!IMPORTANT]
> **Por que `FD_ISSET(fd, &readfds)` é essencial:**
> Sem ela, tentaríamos `recv()` em fds sem dados pendentes (podendo bloquear) ou `accept()` no serverfd sem conexão pendente. O `select()` marca em `readfds` **APENAS** os fds que têm atividade — pular essa verificação quebraria toda a lógica de multiplexação.

---

## Referência da API de Sockets e Multiplexação (System Calls)

Esta é uma "cola" rápida das funções que você usará no exame, com seus argumentos detalhados:

### `int socket(int domain, int type, int protocol)`
Cria um ponto de comunicação (socket) e retorna seu File Descriptor.
*   **`domain`**: A família do protocolo. Usamos `AF_INET` para IPv4.
*   **`type`**: O tipo de comunicação. Usamos `SOCK_STREAM` para conexões TCP confiáveis e orientadas a fluxo.
*   **`protocol`**: O protocolo específico. Usamos `0` para que o sistema escolha automaticamente o protocolo padrão do `type` (que será o TCP).

### `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`
Associa (amarra) o socket recém-criado a um endereço IP e Porta específicos na máquina local.
*   **`sockfd`**: O File Descriptor retornado pelo `socket()`.
*   **`addr`**: Um ponteiro para a estrutura genérica `struct sockaddr`. Nós preenchemos uma `struct sockaddr_in` com IP e Porta, e fazemos o casting com `(const struct sockaddr *)`.
*   **`addrlen`**: O tamanho em bytes da estrutura de endereço passada (ex: `sizeof(servaddr)`).

### `int listen(int sockfd, int backlog)`
Marca o socket como passivo, ou seja, um socket que será usado para "ouvir" e aceitar conexões que chegam.
*   **`sockfd`**: O File Descriptor do servidor.
*   **`backlog`**: O tamanho máximo da fila de clientes esperando para conectar. Usamos `10` (herdado do boilerplate). Se mais clientes tentarem conectar simultaneamente, serão rejeitados.

### `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)`
Aceita a primeira conexão que está esperando na fila do `listen`, criando um *novo* socket exclusivo para se comunicar com esse cliente.
*   **`sockfd`**: O socket passivo do servidor que está ouvindo.
*   **`addr`**: Ponteiro onde o sistema escreveria o IP/Porta do cliente que acabou de conectar. Passamos `NULL` pois não precisamos dessa informação no exame.
*   **`addrlen`**: Ponteiro para o tamanho do `addr`. Passamos `NULL`.
*   **Retorno**: Um **novo** File Descriptor usado apenas para falar com aquele cliente específico.

### `int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)`
A função principal de multiplexação. Pausa o programa e espera até que pelo menos um dos sockets esteja "pronto" para ler ou escrever.
*   **`nfds`**: O número do maior File Descriptor monitorado **mais um** (`maxfds + 1`).
*   **`readfds`**: Conjunto de fds que você quer monitorar para ver se há **dados novos para ler** (ou se há novas conexões no caso do `serverfd`).
*   **`writefds`**: Conjunto de fds monitorados para ver se há **espaço livre para escrever**. No exame, **passamos `NULL`** para simplificar o código, pois a Moulinette nunca causa gargalos de rede que bloqueariam o envio.
*   **`exceptfds`**: Conjunto para monitorar erros excepcionais. Usamos `NULL` (não nos importamos).
*   **`timeout`**: Tempo máximo para o select esperar. Usamos `NULL` para que ele espere infinitamente (bloqueante) até algo acontecer.

### `ssize_t recv(int sockfd, void *buf, size_t len, int flags)`
Lê dados (mensagens) enviados pelo cliente.
*   **`sockfd`**: O fd do cliente específico.
*   **`buf`**: O buffer (array de char) onde os dados recebidos serão guardados.
*   **`len`**: O número máximo de bytes a ler. Passamos `sizeof(buffer) - 1` para garantir espaço para o `\0`.
*   **`flags`**: Modificadores especiais. Usamos `0` (sem modificadores).
*   **Retorno**: O número de bytes lidos. Se retornar `0`, significa que o cliente desconectou (EOF). Se `< 0`, erro.

### `ssize_t send(int sockfd, const void *buf, size_t len, int flags)`
Envia dados (mensagens) para um cliente.
*   **`sockfd`**: O fd do cliente de destino.
*   **`buf`**: A string/dados que você quer enviar.
*   **`len`**: A quantidade de bytes a enviar (geralmente `strlen(buf)`).
*   **`flags`**: Modificadores. Usamos `0`.

### Macros do `fd_set` (Para usar com o `select`)
Um `fd_set` é apenas um array de bits por baixo dos panos. Estas macros o manipulam:
*   **`FD_ZERO(&set)`**: Zera todo o conjunto. Essencial para inicializar o `master` e remover lixo de memória.
*   **`FD_SET(fd, &set)`**: Liga o bit do `fd` no conjunto (adiciona ele ao monitoramento).
*   **`FD_CLR(fd, &set)`**: Desliga o bit do `fd` (remove ele do monitoramento).
*   **`FD_ISSET(fd, &set)`**: Pergunta se o bit do `fd` está ligado. O `select` usa essa macro para nos responder quais clientes estão prontos!
*   **`FD_SETSIZE`**: Não é uma função, mas uma constante nativa do Linux (geralmente 1024) que define o tamanho em bits da struct `fd_set` e dita o limite absoluto de file descriptors que o `select()` consegue suportar de uma vez.

### Outras Funções da LibC (Úteis)
*   **`void bzero(void *s, size_t n)`**: Preenche um bloco de memória com zeros. No `mini_serv`, usamos para "limpar" a struct `servaddr` antes de configurá-la. É crucial porque structs na stack nascem com lixo de memória, o que faria o `bind()` falhar. (Dica: é a versão legada do `memset`).
*   **`int sprintf(char *str, const char *format, ...)`**: Funciona igual ao `printf`, mas ao invés de imprimir na tela, ele "imprime" (grava) o texto formatado dentro de uma string (array de `char`). É a forma mais fácil e rápida de colocar o `id` (int) do cliente dentro da frase `"server: client %d just arrived\n"` sem precisar programar um `itoa` na mão.

---

## 7. Principais Funções Auxiliares (Do [main.c](../subject/main.c) Fornecido)

### [int extract_message(char \*\*buf, char \*\*msg)](../subject/main.c#L8-L33)
-   **Fornecida**: Sim — mas vamos alterá-la para morrer em caso de erro.
-   **Papel**: Verifica `*buf` por um `\n`.
-   **Retornos**:
    | Valor | Significado |
    |---|---|
    | `1` | Encontrou uma linha → copiada para `*msg` (incluindo `\n`), resto fica em `*buf` |
    | `0` | Sem linha completa ainda (dados parciais ou vazio) |
    -   **Dica**: Sempre chame em um loop `while`: `while (extract_message(...) == 1)`

### [char \*str_join(char \*buf, char \*add)](../subject/main.c#L35-L53)
-   **Fornecida**: Sim — mas vamos alterá-la para morrer em caso de erro.
-   **Papel**: Concatena `add` a `buf`.
-   **COMPORTAMENTO CRÍTICO**: Chama `free(buf)` internamente!
-   **Uso**: `msg = str_join(msg, buffer);` — o ponteiro `msg` antigo é **inválido** após esta chamada.

---

## 8. Dicas e Armadilhas do Exame

### Vazamentos de Memória (causa mais comum de reprovação)
-   Sempre `free(clients[fd].msg)` e defina como `NULL` quando um cliente desconectar.
-   Sempre `free(msg)` dentro do loop `processMessages` após enviar.
-   `str_join` já libera `buf` — não faça double-free!

### Destrutividade do `select`
-   **Nunca** passe `&master` diretamente para o `select`. Você **deve** copiá-lo para `readfds` a cada iteração.

### Rastreamento do Max FD
-   O primeiro argumento do `select` é `maxfds + 1`. Atualize `maxfds` a cada `accept`.
-   Nota: não decrementamos `maxfds` na desconexão (leve ineficiência, mas correto e mais simples).

### `write` vs `send`
-   Ambos funcionam para sockets. `send` é específico para sockets e tecnicamente mais correto.
-   Para mensagens em stderr, você deve usar `write` (não é um socket).

### Verificação de Valores de Retorno
-   Verifique `malloc`/`calloc`, `socket`, `bind`, `listen`, `select`, `accept` por erros.
-   O enunciado exige `Fatal error\n` para todas as falhas de chamada de sistema.
-   Ao invés de checar retornos de erro na main, injetamos `print_error(NULL)` diretamente no `calloc`/`malloc` da `extract_message` e `str_join`.

### Armadilhas de Formatação
-   `server: client %d just arrived\n` — note o espaço após `:` e antes de `client`
-   `server: client %d just left\n` — mesmo padrão
-   `client %d: ` — note o espaço após `:`

---

## 9. Como Treinar para a Prova (Memória Muscular)

O exame da 42 não avalia apenas se você "entende" os conceitos, ele avalia a sua **Memória Muscular** sob estresse. Siga este método de treino para garantir a sua aprovação:

### A Regra de Ouro: Apague tudo se errar
Nunca "conserte" o seu código olhando a cola. Se você travar ou o código não compilar, você pode olhar o guia para ver onde errou. Mas o seu castigo deve ser **apagar a função inteira (ou o arquivo) e digitar de novo do zero**. Só assim o seu cérebro cria o caminho neural correto para não errar no dia.

### O Treino Modular
Siga a mesma estratégia das Fases da próxima seção. Treine em blocos isolados, avançando só quando o bloco atual fluir sem travar:
1. **Bloco A (O Setup Básico):** Decore os 3 headers extras, as variáveis globais e a struct `t_client`.
2. **Bloco B (As Utilitárias):** Pratique escrever `print_error`, `sendAll` e `processMessages`. Elas são curtas e fáceis de memorizar.
3. **Bloco C (O Setup e o Coração):** Treine a `init_server` e o monstro: `run_server` (o `while(1)`, o `select`, e o `for` do `FD_ISSET`).
4. **Bloco D (As Ramificações):** Por fim, treine a `accept_client` e a `handle_client`.

### O "Simulado 42"
Quando sentir que domina os blocos, abra um editor cru (sem plugins), copie apenas o `main.c` original, coloque um cronômetro de 45 minutos e tente fazer tudo do zero. Teste conectando dois terminais com `nc 127.0.0.1 8080`. Se você seguir esse método, terminará o exame real em menos de 15 minutos!

---

## 10. Estratégia Passo a Passo (Dia do Exame)

Siga esta ordem para escrever o código do zero no exame:

### Fase 1: Copiar o código fornecido (2 min)
1. Copie `extract_message` e `str_join` do `main.c` fornecido — e altere as falhas de alocação de memória para chamarem `print_error(NULL)` diretamente.
2. Adicione os headers extras (e por que são necessários):
    - `<sys/select.h>`: Para usar o `select` e os macros `fd_set`, `FD_ZERO`, etc.
    - `<stdlib.h>`: Para gerenciamento de memória (`malloc`, `free`) e `exit()`.
    - `<stdio.h>`: Exclusivamente para usar a função `sprintf`.

### Fase 2: Globais + struct (1 min)
```c
typedef struct s_client { int id; char *msg; } t_client;
t_client clients[FD_SETSIZE];
char buffer_msg[128];
int serverfd = -1, maxfds = 0, current_id = -1;
fd_set master, readfds;
```

### Fase 3: Utilitárias e Erros (2 min)
*(Escrevemos elas primeiro para poder programar o resto do arquivo de cima para baixo sem precisar de protótipos)*
1. Crie `print_error()` — faz write no stderr e exit(1).
2. Crie `sendAll()` — loop 0..maxfds, verifica se está no master, ignora serverfd e o remetente, e dá `send()`.
3. Crie `processMessages()` — sprintf do prefixo "client X:", loop com `extract_message`, envia usando `sendAll`, e dá `free(msg)`.

### Fase 4: O Setup (init_server) (2 min)
1. Crie a `init_server(int port)` reaproveitando o boilerplate do `main.c` original (socket, bzero, bind, listen). Nela, inicialize `serverfd`, atualize `maxfds` e limpe o set `master`.

### Fase 5: As Ramificações de Rede (4 min)
*(Escrevemos antes do run_server para que ele possa chamá-las direto)*
1. Crie `accept_client()`: Roda o `accept`, joga no set `master`, registra o ID e a string nula do novo cliente, e avisa a sala que ele chegou via `sendAll()`.
2. Crie `handle_client(fd)`: Usa o `recv`. Se bytes <= 0, o cliente desconectou (limpa do master, fecha socket, avisa a sala). Se vier texto, concatena com `str_join` e passa para `processMessages()`.

### Fase 6: O Coração do Servidor (run_server) (3 min)
1. Crie a `run_server()` contendo o loop `while(1)`.
2. Espelhe `master` para `readfds` e chame o `select`.
3. Faça o loop `for` interno (0 até `maxfds`) escutando o `FD_ISSET`.
4. Se acender a luz do `serverfd`, chame `accept_client()`. Senão, chame `handle_client(fd)`.

### Fase 7: Função Main (1 min)
1. Escreva uma `main` limpa e curta no final do arquivo: valida `argc != 2`, chama `init_server()` passando a porta, e, em seguida, chama `run_server()`.

### Fase 8: Testar (5 min)
```bash
gcc -Wall -Wextra -Werror pt/array/mini_serv.c -o mini_serv
./mini_serv 8080
# Em outro terminal:
nc 127.0.0.1 8080
# Em mais outro terminal:
nc 127.0.0.1 8080
```

---

## 11. Fluxo Lógico (Resumo)

```text
socket() → bind() → listen()
FD_ZERO & FD_SET(serverfd)

while(1) {
    readfds = master
    select(max + 1, &readfds, NULL, ...)

    for (fd in 0..max) {
        if (!FD_ISSET(fd, &readfds)) continue

        if (fd == server) {
            accept() → newfd
            FD_SET(newfd, &master)
            clients[newfd].id = ++current_id
            clients[newfd].msg = NULL
            atualiza maxfds
            sendAll("server: client X just arrived\n")
        } else {
            bytes = recv()
            if <= 0 {
                sendAll("server: client X just left\n")
                FD_CLR & close & free(clients[fd].msg)
            } else {
                buffer[bytes] = '\0'
                clients[fd].msg = str_join(clients[fd].msg, buffer)
                while (extract_message) {
                    sendAll("client X: ")
                    sendAll(msg)
                    free(msg)
                }
            }
        }
    }
}
```

---

## 11. Verificação de Conhecimento (Quiz)

Teste seu conhecimento com perguntas curtas. Se você souber responder, estará pronto para defender seu código na avaliação!

<details>
<summary><b>1. Por que é necessário usar <code>extract_message</code> no recebimento de dados via TCP?</b></summary>
O TCP é um protocolo de fluxo (stream). As mensagens podem chegar fragmentadas ou agrupadas. A função <code>extract_message</code> garante que o servidor processe uma linha completa de cada vez.
</details>

<details>
<summary><b>2. Qual é o comportamento crítico da função <code>str_join</code> em relação ao gerenciamento de memória?</b></summary>
Ela dá <code>free()</code> automaticamente no ponteiro passado como primeiro argumento após a concatenação, exigindo cuidado para não causar <i>double-free</i> ou vazamentos.
</details>

<details>
<summary><b>3. O que deve ser feito se <code>recv()</code> retornar o valor 0?</b></summary>
Um retorno de 0 indica que o cliente fechou a conexão (EOF). O servidor deve notificar os outros usuários ("just left"), remover o cliente do conjunto <code>master</code>, fechar seu socket (fd) e dar <code>free()</code> no seu buffer.
</details>

<details>
<summary><b>4. Por que não se pode passar o conjunto <code>master</code> diretamente para o <code>select()</code>?</b></summary>
Porque o <code>select()</code> é destrutivo. Ele modificará o conjunto para mostrar apenas os descritores prontos. Se você passar o <code>master</code> e ele for modificado, você perderá o registro de todos os outros clientes para as próximas iterações.
</details>

<details>
<summary><b>5. Por que omitimos o <code>writefds</code> no exame?</b></summary>
<div align="left">

Na vida real, usamos <code>writefds</code> para garantir que o SO tem espaço no buffer TCP do cliente antes de chamar <code>send()</code>, evitando que o servidor congele (bloqueie) aguardando clientes lentos. Como o exame testa o código localmente e os "clientes" da Moulinette leem os pequenos blocos de texto instantaneamente, não há risco do buffer encher. Omitir o <code>writefds</code> e passar `NULL` simplifica brutalmente o código sem riscos.

</div>
</details>

---

## 12. Glossário de Termos Chave

| Termo | Definição |
|---|---|
| **Multiplexação de I/O** | Técnica que permite que um único processo monitore múltiplos canais de I/O ao mesmo tempo (via `select()`). |
| **Descritor de Arquivo (fd)** | Um índice inteiro não negativo mantido pelo kernel que aponta para um arquivo ou socket aberto por um processo. |
| **Não-bloqueante (Non-blocking)** | Um modo de operação onde uma chamada de sistema retorna imediatamente se não puder completar a tarefa, em vez de pausar a execução. |
| **Broadcasting** | Envio de uma mensagem recebida de um participante para todos os outros participantes da rede simultaneamente. |
| **Socket** | Um ponto final de comunicação bidirecional entre dois processos em uma rede. |
| **EOF (End Of File)** | Condição atingida quando não há mais dados a serem lidos de um fluxo, frequentemente indicando desconexão em sockets. |
