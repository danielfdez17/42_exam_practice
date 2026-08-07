/*
** =============================================================================
** SOLUÇÃO: Baseada em Struct (usando array de structs para dados de clientes)
** IDIOMA: Comentários em Português
** =============================================================================
*/

/*
** mini_serv - Servidor de chat minimalista (Exam Rank 06)
**
** Diferenças em relação ao main.c fornecido:
** - Adicionados os headers <sys/types.h>, <sys/select.h>, <stdlib.h> e <stdio.h>
**   que são necessários para select(), tipos como ssize_t, malloc/calloc/free,
**   sprintf e atoi.
** - O main.c original aceitava apenas uma conexão (accept direto).
**   Aqui implementamos multiplexação com select() para múltiplos clientes.
** - Removidos todos os printf do main.c (o servidor não imprime nada em
**   execução normal; erros são tratados por print_error() escrevendo em stderr).
** - A porta deixou de ser hardcoded (8081) e passou a ser recebida via argv[1].
** - sockfd foi substituído pela variável global serverfd.
** - connfd e len foram removidos; a aceitação de conexões acontece dentro do
**   loop de select().
*/
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>  // Adicionado: select(), fd_set, FD_ZERO, FD_SET, FD_CLR, FD_ISSET
#include <stdlib.h>      // Adicionado: malloc, calloc, free, atoi, exit
#include <stdio.h>       // Adicionado: sprintf

/*
** ========================== VARIÁVEIS GLOBAIS ==========================
*/

// O uso de uma struct melhora a coesão dos dados e a organização em
// comparação com arrays paralelos.
typedef struct s_client {
	int		id;
	char	*msg;
} t_client;

/*
** Uso:
** clients[fd].id = 1;
** clients[fd].msg = str_join(clients[fd].msg, buffer);
*/

// Mapeia fd -> dados do cliente (ID sequencial e buffer de mensagem).
// Tamanho 65536: valor máximo teórico de um fd em Linux (previne
// segfaults caso o sistema permita fds maiores que FD_SETSIZE).
t_client clients[FD_SETSIZE];

// Buffer auxiliar para montar strings formatadas com sprintf().
// Tipo char[4096]: tamanho fixo suficiente para prefixos como
// "server: client X just arrived\n" ou "client X: ".
// 128 é um tamanho seguro para qualquer mensagem de controle do servidor.
char buffer_msg[128];

// File descriptor do socket do servidor.
// Tipo int: padrão para fds em POSIX. Inicializado em -1 (inválido)
// para indicar que ainda não foi criado.
// No main.c original era a variável local sockfd.
int serverfd = -1;

// Maior file descriptor em uso, utilizado como (maxfds + 1) no select().
// Tipo int: compatível com o 1º argumento de select (nfds).
// Atualizado a cada nova conexão aceita.
int maxfds = 0;

// Contador sequencial de IDs. Começa em -1 para que o primeiro
// cliente receba ID 0 após o pré-incremento (++current_id).
// Tipo int: inteiros simples e sequenciais.
int current_id = -1;

// Conjuntos de file descriptors (fd_sets) usados pelo select().
// master: armazena a lista de TODOS os fds atualmente conectados (server + clients).
// readfds: cópia temporária passada para o select(), pois o select()
// modifica os conjuntos para indicar apenas os fds que estão prontos.
// Mantemos como globais para facilitar o acesso na função sendAll().
fd_set master;
fd_set readfds;

/**
 * @brief Imprime um erro em stderr e encerra o programa (exit 1).
 *
 * Funde a funcionalidade de impressão e saída num único local para
 * economizar código durante o exame. Se a mensagem for NULL,
 * assume o erro genérico exigido pelo enunciado.
 *
 * @param msg Mensagem a ser impressa, ou NULL para "Fatal error\n"
 */
void print_error(char *msg)
{
	if (msg)
		write(2, msg, strlen(msg));
	else
		write(2, "Fatal error\n", 12);
	exit(1);
}

/*
** =================== FUNÇÕES FORNECIDAS (main.c) ====================
** As funções extract_message() e str_join() originais do main.c foram
** modificadas (hackeadas) aqui para chamarem print_error(NULL) diretamente
** caso malloc/calloc falhe. Isso limpa a lógica do main loop.
*/

/**
 * @brief Extrai a próxima mensagem terminada em '\n' do buffer.
 *
 * @param buf Ponteiro para o buffer acumulado do cliente (modificado in-place).
 * @param msg Ponteiro onde a mensagem extraída será armazenada (inclui o '\n').
 * @return 1 se encontrou uma mensagem, 0 se não há '\n', -1 em erro de alocação.
 */
int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int		i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				print_error(NULL); // HACK: Original devolvia return (-1). Trocamos para exit direto.
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Concatena 'add' ao final de 'buf', liberando o 'buf' antigo.
 *
 * @param buf Buffer existente (pode ser NULL na primeira chamada).
 * @param add String a ser concatenada.
 * @return O novo buffer alocado ou NULL em caso de falha.
 */
char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		print_error(NULL); // HACK: Original devolvia return (0). Trocamos para exit direto.
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

/*
** =================== FUNÇÕES AUXILIARES (criadas) ====================
** Funções utilitárias que não existiam no main.c original.
*/


/**
 * @brief Envia uma mensagem para todos os clientes conectados, exceto o remetente e o socket do servidor.
 *
 * Percorre de 0 até maxfds verificando o set `master` (que contém todos os
 * clientes atualmente ativos). Não usamos writefds/checagem de bloqueio no exame.
 *
 * @param sender File descriptor do cliente que enviou a mensagem (será ignorado).
 * @param msg String a ser enviada.
 */
void sendAll(int sender, char *msg)
{
	int len = strlen(msg);
	for (int fd = 0; fd <= maxfds; fd++)
	{
		if (FD_ISSET(fd, &master) && fd != serverfd && fd != sender)
			send(fd, msg, len, 0);
	}
}

/**
 * @brief Extrai e repassa mensagens completas do buffer de um cliente.
 *
 * O TCP pode fragmentar ou agrupar dados, então o buffer pode conter zero,
 * uma ou várias mensagens delimitadas por '\n'.
 *
 * @param sender File descriptor do cliente cujas mensagens serão processadas.
 */
void processMessages(int sender)
{
	// Ponteiro para armazenar cada mensagem extraída por extract_message().
	// Tipo char*: aponta para memória alocada internamente por extract_message(),
	// que deve ser liberada com free() após o uso.
	char *msg;

	// Monta o prefixo "client <id>: " no buffer global.
	// Feito uma vez antes do loop, pois o ID não muda entre mensagens.
	sprintf(buffer_msg, "client %d: ", clients[sender].id);

	// Tipo int: retorno de extract_message (1=mensagem, 0=incompleta, -1=erro).
	// extract_message dá exit(1) em falha, não precisamos checar retorno de erro.
	while (extract_message(&clients[sender].msg, &msg) == 1)
	{
		sendAll(sender, buffer_msg); // Envia o prefixo "client X: "
		sendAll(sender, msg);        // Envia o corpo da mensagem (inclui '\n')
		free(msg);                   // Libera a memória alocada por extract_message()
	}
}

/*
** ==================== LÓGICA DE REDE EXTRAÍDA ====================
** Funções abaixo foram extraídas da função main() original para
** facilitar a leitura e modularidade do código.
*/

/**
 * @brief Configura o socket do servidor, faz bind e listen.
 *
 * @param port_str String contendo a porta passada por argv[1].
 */
void init_server(char *port_str)
{
	// Tipo struct sockaddr_in: estrutura POSIX para endereço IPv4.
	// Contém família (AF_INET), IP e porta do servidor.
	// No main.c original havia também 'struct sockaddr_in cli' para o accept,
	// que foi removida completamente, pois agora passamos NULL direto no accept().
	struct sockaddr_in servaddr;

	// Cria o socket TCP (SOCK_STREAM) para IPv4 (AF_INET).
	// No main.c era: sockfd = socket(...) com printf em caso de sucesso/erro.
	// Aqui usamos serverfd (global) e tratamos erro apenas com print_error().
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd == -1)
		print_error(NULL);

	// Zera a estrutura para evitar lixo de memória (mesmo que no main.c).
	bzero(&servaddr, sizeof(servaddr));

	// Configura o endereço do servidor (mesmo que no main.c, exceto a porta).
	servaddr.sin_family = AF_INET;            // IPv4
	servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1 em network byte order
	// No main.c: htons(8081) hardcoded.
	// Aqui: htons(atoi(port_str)) para usar a porta passada como argumento.
	servaddr.sin_port = htons(atoi(port_str));

	// Associa o socket ao endereço/porta e coloca em modo de escuta.
	// No main.c havia printf de sucesso; aqui apenas tratamos erro com print_error().
	if ((bind(serverfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		close(serverfd); // Opcional
		print_error(NULL);
	}
	// listen(fd, backlog): backlog = 10 define o tamanho máximo da fila de
	// conexões pendentes (mesmo valor do main.c original).
	if (listen(serverfd, 10) != 0)
	{
		close(serverfd); // Opcional
		print_error(NULL);
	}
}

/**
 * @brief Aceita uma nova conexão e registra o cliente no servidor.
 *
 * Chamada quando o select() indica que serverfd está pronto para leitura.
 */
void accept_client()
{
	// O accept() pede ponteiros para armazenar o IP/Porta de quem conectou.
	// Como o exame não exige saber isso, a página 'man 2 accept' permite passar NULL.
	// Isso é superior pois não criamos variáveis inutilizadas e o código fica mais limpo.
	/*
	** Alternativa educacional (como seria se precisássemos do IP):
	** struct sockaddr addr;
	** socklen_t lensock = sizeof(addr);
	** int newfd = accept(serverfd, &addr, &lensock);
	*/
	int newfd = accept(serverfd, NULL, NULL);
	if (newfd == -1)
		return ; // O main.c fornecido dá exit() aqui, mas isso mataria o servidor
		         // inteiro se apenas um cliente falhasse ao conectar. Apenas ignoramos.

	/*
	** OPCIONAL: próximo `if`
	** Verifica se o fd excede FD_SETSIZE (geralmente 1024).
	** select() usa fd_set com tamanho fixo; fds >= FD_SETSIZE
	** causam undefined behavior em FD_SET/FD_ISSET.
	** Em produção, use poll()/epoll() para evitar essa limitação.
	*/
	if (newfd >= FD_SETSIZE)
	{
		close(newfd);
		return ;
	}

	FD_SET(newfd, &master);      // Registra para monitoramento
	clients[newfd].id = ++current_id;   // Atribui ID sequencial
	clients[newfd].msg = NULL;    // Inicializa buffer vazio
	if (newfd > maxfds)          // Atualiza limite do select()
		maxfds = newfd;

	// Notifica os demais clientes sobre a nova conexão.
	sprintf(buffer_msg, "server: client %d just arrived\n", clients[newfd].id);
	sendAll(newfd, buffer_msg);
}

/**
 * @brief Recebe e processa dados enviados por um cliente já conectado.
 *
 * Trata também a desconexão caso o cliente feche o socket ou ocorra erro.
 *
 * @param fd File descriptor do cliente que possui dados para leitura.
 */
void handle_client(int fd)
{
	// ===== RECEBIMENTO DE DADOS =====
	// Tipo char[4096]: buffer local para receber dados do recv().
	// Tamanho 4096: valor comum para buffers de rede; suficiente para
	// mensagens típicas sem ser excessivo para a stack.
	char buffer[4096];
	// Tipo ssize_t: O retorno correto de recv(), que suporta tamanhos de memória
	// e valores negativos (-1 em erro). Evita possíveis overflows de 'int'.
	// sizeof(buffer) - 1: reserva 1 byte para o '\0' terminador.
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

	// ===== DESCONEXÃO =====
	// recv() retorna 0 quando o cliente fecha a conexão de forma ordenada,
	// ou -1 em caso de erro. Em ambos os casos, desconectamos o cliente.
	if (bytes <= 0)
	{
		sprintf(buffer_msg, "server: client %d just left\n", clients[fd].id);
		sendAll(fd, buffer_msg);     // Notifica os demais
		FD_CLR(fd, &master);         // Remove do monitoramento
		close(fd);                   // Fecha o socket do cliente
		// Libera o buffer de mensagens acumuladas do cliente.
		// free(NULL) é seguro (no-op) pelo padrão C — não precisa de guard.
		free(clients[fd].msg);
		clients[fd].msg = NULL;
		return ;
	}

	// ===== PROCESSAMENTO DE MENSAGEM =====
	// Adiciona '\0' para tratar os dados recebidos como string C.
	buffer[bytes] = '\0';
	// Concatena ao buffer acumulado do cliente com str_join().
	// Necessário porque o TCP pode fragmentar uma mensagem em múltiplos
	// recv(), ou agrupar várias mensagens em um único recv().
	// O extract_message() em processMessages() só extrai linhas completas.
	// str_join dá exit(1) em falha, não precisamos checar msg == NULL
	clients[fd].msg = str_join(clients[fd].msg, buffer);
	processMessages(fd);
}

/**
 * @brief Contém o loop infinito principal do select().
 *
 * Gerencia os conjuntos de file descriptors, aguarda por eventos
 * em sockets prontos e roteia para conexão (accept) ou dados (handle).
 */
void run_server()
{
	/*
	** Inicialização do select() — trecho totalmente novo em relação ao main.c.
	** O main.c original fazia um único accept() e terminava.
	** Aqui usamos select() para multiplexar múltiplas conexões.
	*/
	// serverfd geralmente vale 3 (após stdin=0, stdout=1, stderr=2).
	maxfds = serverfd;
	// FD_ZERO zera todos os bits do fd_set, removendo qualquer lixo.
	FD_ZERO(&master);
	// Registra o socket do servidor no conjunto para detectar novas conexões.
	FD_SET(serverfd, &master);

	// Loop principal — executa indefinidamente até o processo ser encerrado.
	// Todo o trecho abaixo é novo em relação ao main.c.
	while (42)
	{
		// Cópia dos fd_sets master para esta iteração.
		// O select() modifica os conjuntos passados, removendo fds que
		// não estão prontos. Readfds é recriado a partir do master a cada iteração.
		readfds = master;
		// select(nfds, readfds, writefds, exceptfds, timeout):
		//   nfds:      maxfds + 1 (maior fd + 1, define o range de busca)
		//   readfds:   fds monitorados para leitura (dados disponíveis ou nova conexão)
		//   writefds:  NULL (ignorado propositalmente no exame para reduzir código)
		//   exceptfds: NULL (não monitoramos exceções)
		//   timeout:   NULL (bloqueia indefinidamente até um evento ocorrer)
		// Retorno: número de fds prontos, ou -1 em caso de erro.
		int ret = select(maxfds + 1, &readfds, NULL, NULL, NULL);
		// select() retornando -1 é erro fatal: sem select funcional, o servidor
		// não consegue multiplexar — não há como continuar.
		// Um "continue" aqui faria um busy-loop infinito com readfds/writefds
		// corrompidos, pois select() não o preencheu corretamente.
		// Obs: Não fechamos os FDs manualmente para economizar código. Confiamos
		// que o Kernel limpará todos os recursos instantaneamente após o exit(1).
		if (ret == -1)
			print_error(NULL);

		// Percorre de 0 até maxfds verificando quais fds estão prontos.
		// Diferente do epoll (que retorna apenas os fds prontos), o select()
		// exige que verifiquemos cada fd individualmente com FD_ISSET.
		for (int fd = 0; fd <= maxfds; fd++)
		{
			// Se o fd não está marcado em readfds, não há dados para ler.
			// Verificação essencial: sem ela, tentaríamos recv() em fds sem
			// dados pendentes (podendo bloquear) ou accept() no serverfd
			// sem conexão pendente. O select() marca em readfds APENAS os
			// fds que têm atividade; ignorar isso quebraria toda a lógica.
			if (!FD_ISSET(fd, &readfds))
				continue ;

			// ===== NOVA CONEXÃO =====
			// Se o fd pronto é o do servidor, há um cliente querendo conectar.
			// No main.c original, o accept() era feito uma única vez fora de loop.
			// Aqui, o accept() foi extraído para a função accept_client().
			if (fd == serverfd)
				accept_client();
			else // ===== CLIENTE EXISTENTE (DADOS OU DESCONEXÃO) =====
				handle_client(fd);
		}
	}
}

/**
 * @brief Ponto central de execução do programa.
 *
 * ========================== MUDANÇAS EM RELAÇÃO AO MAIN.C ORIGINAL ==========================
 * - Assinatura: main() -> main(int argc, char **argv) para receber a porta.
 * - Variáveis locais removidas: sockfd (substituído por serverfd global),
 *   connfd, cli e len (removidos completamente; accept no loop de
 *   select agora usa parâmetros NULL pois não precisamos do endereço do cliente).
 * - Porta: htons(8081) -> htons(atoi(argv[1])) (configurável via argumento).
 * - Tratamento de erros: printf + exit(0) -> print_error() que escreve em stderr
 *   e sai com código 1, conforme exigido pelo enunciado.
 * - Estrutura cli removida: passamos NULL diretamente no accept.
 * - Modularização máxima: todo o setup (socket/bind/listen) foi para
 *   init_server, e o event loop do select() foi para run_server.
 * - Loop principal "escondido" sob run_server para uma main super limpa.
 *
 * @param argc Número de argumentos.
 * @param argv Argumentos (espera-se a porta no índice 1).
 * @return 0 em caso de sucesso (teoricamente inalcançável por conta do loop infinito).
 */
int main(int argc, char **argv)
{
	// Validação de argumentos (não existe no main.c original).
	// O enunciado exige exatamente 1 argumento (a porta).
	if (argc != 2)
		print_error("Wrong number of arguments\n");

	// Configuração e inicialização do socket do servidor
	init_server(argv[1]);

	// Inicia o loop infinito do select() para tratar conexões
	run_server();

	/*
	** Este return nunca é atingido pois run_server possui um while(1).
	**
	** Código original do main.c (removido):
	** O accept() que existia na main foi centralizado em accept_client()
	** chamado dentro do select() em run_server(). Um único accept simples
	** se tornou um sistema de repetição contínua para multiplexação.
	**   len = sizeof(cli);
	**   connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	**   if (connfd < 0) { printf("server acccept failed...\n"); exit(0); }
	**   else printf("server acccept the client...\n");
	*/
	return (0);
}

