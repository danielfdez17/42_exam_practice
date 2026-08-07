/*
** =============================================================================
** SOLUÇÃO: Baseada em Array (usando array de strings para buffers de clientes)
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
**   execução normal; erros são tratados por print_error(NULL) escrevendo em stderr).
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
#include <sys/types.h>   // Adicionado: tipos como ssize_t usados por recv/select
#include <sys/select.h>  // Adicionado: select(), fd_set, FD_ZERO, FD_SET, FD_CLR, FD_ISSET
#include <stdlib.h>      // Adicionado: malloc, calloc, free, atoi, exit
#include <stdio.h>       // Adicionado: sprintf


/*
** ========================== VARIÁVEIS GLOBAIS ==========================
**
** Usamos arrays indexados pelo file descriptor (fd) do cliente.
** Tipo int[] / char*[]: arrays simples e de acesso O(1) pelo fd.
** Tamanho 65536: corresponde ao valor máximo teórico de um fd em Linux
** (definido por /proc/sys/fs/file-max). Na prática, o select() possui
** um limite de FD_SETSIZE (geralmente 1024), mas o tamanho maior
** previne segfaults caso o sistema permita fds mais altos.
**
** Alternativa: usar uma struct para agrupar dados do cliente.
** Exemplo:
** typedef struct s_client {
**     int     id;
**     char    *msg;
** } t_client;
**
** t_client clients[65536];
**
** Uso:
** clients[fd].id = 1;
** clients[fd].msg = str_join(clients[fd].msg, buffer);
*/

// Mapeia fd -> ID sequencial do cliente.
// Exemplo: ids[7] = 0 significa que o cliente com fd 7 recebeu o ID 0.
// Tipo int: IDs são inteiros simples e sequenciais.
int ids[65536];

// Mapeia fd -> buffer de mensagem acumulada do cliente.
// Tipo char*: ponteiro para string alocada dinamicamente por str_join().
// Inicializado como NULL; cresce conforme dados são recebidos via recv().
char *client_msg[65536];

// Buffer auxiliar para montar strings formatadas com sprintf().
// Tipo char[4096]: tamanho fixo suficiente para prefixos como
// "server: client X just arrived\n" ou "client X: ".
// 4096 é um tamanho seguro para qualquer mensagem de controle do servidor.
char buffer_msg[4096];

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
// Mantémos como globais para facilitar o acesso na função sendAll().
fd_set master;
fd_set readfds;
// print_error(): imprime um erro em stderr e encerra o programa (exit 1).
// Substitui os múltiplos printf + exit(0) do main.c original.
// Se msg for NULL, imprime "Fatal error\n" (erros de sistema/alocação).
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

// extract_message(): extrai a próxima mensagem terminada em '\n' do buffer.
// Retorna 1 se encontrou uma mensagem, 0 se não há '\n', -1 em erro de alocação.
// Parâmetros:
//   buf (char**): ponteiro para o buffer acumulado do cliente (modificado in-place).
//   msg (char**): ponteiro onde a mensagem extraída será armazenada (inclui o '\n').
int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

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

// str_join(): concatena 'add' ao final de 'buf', liberando o 'buf' antigo.
// Retorna o novo buffer alocado ou NULL em caso de falha.
// Parâmetros:
//   buf (char*): buffer existente (pode ser NULL na primeira chamada).
//   add (char*): string a ser concatenada.
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


// sendAll(): envia uma mensagem para todos os clientes conectados,
// exceto o remetente e o socket do servidor.
// Percorre de 0 até maxfds verificando o set `master`.
// Parâmetros:
//   sender (int): fd do cliente que enviou a mensagem (será ignorado).
//   msg (char*): string a ser enviada.
// Verifica FD_ISSET(fd, &master) para confirmar se o cliente está ativo.
void sendAll(int sender, char *msg)
{
	int len = strlen(msg);
	for (int fd = 0; fd <= maxfds; fd++)
	{
		if (FD_ISSET(fd, &master) && fd != serverfd && fd != sender)
			send(fd, msg, len, 0);
	}
}

// processMessages(): extrai e repassa mensagens completas do buffer de um cliente.
// O TCP pode fragmentar ou agrupar dados, então o buffer pode conter zero,
// uma ou várias mensagens delimitadas por '\n'.
// Parâmetros:
//   sender (int): fd do cliente cujas mensagens serão processadas.
void processMessages(int sender)
{
	// Ponteiro para armazenar cada mensagem extraída por extract_message().
	// Tipo char*: aponta para memória alocada internamente por extract_message(),
	// que deve ser liberada com free() após o uso.
	char *msg;

	// Monta o prefixo "client <id>: " no buffer global.
	// Feito uma vez antes do loop, pois o ID não muda entre mensagens.
	sprintf(buffer_msg, "client %d: ", ids[sender]);

	// Tipo int: retorno de extract_message (1=mensagem, 0=incompleta, -1=erro).
	int ret;
	while ((ret = extract_message(&client_msg[sender], &msg)) == 1)
	{
		sendAll(sender, buffer_msg); // Envia o prefixo "client X: "
		sendAll(sender, msg);        // Envia o corpo da mensagem (inclui '\n')
		free(msg);                   // Libera a memória alocada por extract_message()
	}
	// Se extract_message retornou -1, houve falha de alocação (calloc).
	if (ret == -1)
		print_error(NULL);
}

/*
** ========================== FUNÇÃO PRINCIPAL ==========================
**
** Mudanças em relação ao main() do main.c original:
** - Assinatura: main() -> main(int argc, char **argv) para receber a porta.
** - Variáveis locais removidas: sockfd (substituído por serverfd global),
**   connfd e len (accept agora ocorre dentro do loop de select).
** - Porta: htons(8081) -> htons(atoi(argv[1])) (configurável via argumento).
** - Tratamento de erros: printf + exit(0) -> print_error(NULL) que escreve em stderr
**   e sai com código 1, conforme exigido pelo enunciado.
** - Estrutura cli removida: passamos NULL diretamente no accept.
** - Adicionada a lógica de multiplexação com select() no loop infinito.
*/
int main(int argc, char **argv)
{
	// Validação de argumentos (não existe no main.c original).
	// O enunciado exige exatamente 1 argumento (a porta).
	if (argc != 2)
		print_error("Wrong number of arguments\n");

	// Tipo struct sockaddr_in: estrutura POSIX para endereço IPv4.
	// Contém família (AF_INET), IP e porta do servidor.
	// No main.c original havia também 'struct sockaddr_in cli' para o accept,
	// que foi removida completamente, pois agora passamos NULL direto no accept().
	struct sockaddr_in servaddr;

	// Cria o socket TCP (SOCK_STREAM) para IPv4 (AF_INET).
	// No main.c era: sockfd = socket(...) com printf em caso de sucesso/erro.
	// Aqui usamos serverfd (global) e tratamos erro apenas com print_error(NULL).
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd == -1)
		print_error(NULL);

	// Zera a estrutura para evitar lixo de memória (mesmo que no main.c).
	bzero(&servaddr, sizeof(servaddr));

	// Configura o endereço do servidor (mesmo que no main.c, exceto a porta).
	servaddr.sin_family = AF_INET;            // IPv4
	servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1 em network byte order
	// No main.c: htons(8081) hardcoded.
	// Aqui: htons(atoi(argv[1])) para usar a porta passada como argumento.
	servaddr.sin_port = htons(atoi(argv[1]));

	// Associa o socket ao endereço/porta e coloca em modo de escuta.
	// No main.c havia printf de sucesso; aqui apenas tratamos erro com print_error(NULL).
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
	while (1)
	{
		// Cópia dos fd_sets master para esta iteração.
		// O select() modifica os conjuntos passados, removendo fds que
		// não estão prontos. Readfds é recriado a partir do master a cada iteração.
		readfds = master;
		// select(nfds, readfds, writefds, exceptfds, timeout):
		//   nfds:      maxfds + 1 (maior fd + 1, define o range de busca)
		//   readfds:   fds monitorados para leitura (dados disponíveis ou nova conexão)
		//   writefds:  NULL (ignorado no exame para poupar código).
		//   exceptfds: NULL (não monitoramos exceções)
		//   timeout:   NULL (bloqueia indefinidamente até um evento ocorrer)
		// Retorno: número de fds prontos, ou -1 em caso de erro.
		int ret = select(maxfds + 1, &readfds, NULL, NULL, NULL);
		// select() retornando -1 é erro fatal. Não fechamos os FDs manualmente
		// para economizar código, confiando que o Kernel fará isso no exit(1).
		if (ret == -1)
			print_error(NULL);

		// Percorre de 0 até maxfds verificando quais fds estão prontos.
		// Diferente do epoll (que retorna apenas os fds prontos), o select()
		// exige que verifiquemos cada fd individualmente com FD_ISSET.
		for (int fd = 0; fd <= maxfds; fd++)
		{
			// Se o fd não está marcado em readfds, não há dados para ler.
			if (!FD_ISSET(fd, &readfds))
				continue ;

			// ===== NOVA CONEXÃO =====
			// Se o fd pronto é o do servidor, há um cliente querendo conectar.
			// No main.c original, o accept() era feito uma única vez fora de loop.
			// Aqui, cada accept() ocorre dentro do loop de select().
			if (fd == serverfd)
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
					continue ; // O main.c fornecido dá exit() aqui, mas isso mataria o servidor
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
					continue;
				}

				FD_SET(newfd, &master);      // Registra para monitoramento
				ids[newfd] = ++current_id;   // Atribui ID sequencial
				client_msg[newfd] = NULL;    // Inicializa buffer vazio
				if (newfd > maxfds)          // Atualiza limite do select()
					maxfds = newfd;

				// Notifica os demais clientes sobre a nova conexão.
				sprintf(buffer_msg, "server: client %d just arrived\n", ids[newfd]);
				sendAll(newfd, buffer_msg);
				continue ;
			}

			// ===== RECEBIMENTO DE DADOS =====
			// Tipo char[4096]: buffer local para receber dados do recv().
			// Tamanho 4096: valor comum para buffers de rede; suficiente para
			// mensagens típicas sem ser excessivo para a stack.
			char buffer[4096];
			// Tipo ssize_t: retorno de recv() pode ser negativo (-1 = erro).
			// sizeof(buffer) - 1: reserva 1 byte para o '\0' terminador.
			ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

			// ===== DESCONEXÃO =====
			// recv() retorna 0 quando o cliente fecha a conexão de forma ordenada,
			// ou -1 em caso de erro. Em ambos os casos, desconectamos o cliente.
			if (bytes <= 0)
			{
				sprintf(buffer_msg, "server: client %d just left\n", ids[fd]);
				sendAll(fd, buffer_msg);     // Notifica os demais
				FD_CLR(fd, &master);         // Remove do monitoramento
				close(fd);                   // Fecha o socket do cliente
				// Libera o buffer de mensagens acumuladas do cliente.
				// free(NULL) é seguro (no-op) pelo padrão C — não precisa de guard.
				free(client_msg[fd]);
				client_msg[fd] = NULL;
				continue;
			}

			// ===== PROCESSAMENTO DE MENSAGEM =====
			// Adiciona '\0' para tratar os dados recebidos como string C.
			buffer[bytes] = '\0';
			// Concatena ao buffer acumulado do cliente com str_join().
			// Necessário porque o TCP pode fragmentar uma mensagem em múltiplos
			// recv(), ou agrupar várias mensagens em um único recv().
			// O extract_message() em processMessages() só extrai linhas completas.
			client_msg[fd] = str_join(client_msg[fd], buffer);
			if (client_msg[fd] == NULL)
				print_error(NULL);
			processMessages(fd);
		}
	}
	/*
	** Código original do main.c (removido):
	** O accept() que existia aqui foi movido para dentro do loop de select(),
	** permitindo aceitar múltiplas conexões ao longo da execução do servidor.
	**
	** Trecho removido:
	**   len = sizeof(cli);
	**   connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	**   if (connfd < 0) { printf("server acccept failed...\n"); exit(0); }
	**   else printf("server acccept the client...\n");
	*/
}

