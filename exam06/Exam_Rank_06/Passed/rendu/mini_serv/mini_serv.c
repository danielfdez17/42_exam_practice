// #include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_client {
	int		id;
	char	*msg;
} t_client;

t_client	clients[FD_SETSIZE];
char		buffer_msg[128];
int			maxfds = 0, current_id = -1, serverfd = -1;
fd_set 		master, readfds;

void print_error(char *error_msg)
{
	if (error_msg)
		write(2, error_msg, strlen(error_msg));
	else
		write(2, "Fatal error\n", 12);
	exit(1);
}

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
				print_error(NULL);
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
		print_error(NULL);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void send_all(int sender, char *msg)
{
	int len = strlen(msg);
	for (int fd = 0; fd <= maxfds; fd++)
	{
		if (FD_ISSET(fd, &master) && fd != serverfd && fd != sender)
			send(fd, msg, len, 0);
	}
}

void process_messages(int sender)
{
	char *msg;
	sprintf(buffer_msg, "client %d: ", clients[sender].id);
	while (extract_message(&clients[sender].msg, &msg) == 1)
	{
		send_all(sender, buffer_msg);
		send_all(sender, msg);
		free(msg);
	}
}

void accept_client()
{
	int newfd = accept(serverfd, NULL, NULL);
	if (newfd == -1)
		return ;
	if (newfd >= FD_SETSIZE)
	{
		close(newfd);
		return ;
	}
	if (newfd > maxfds)
		maxfds = newfd;
	FD_SET(newfd, &master);
	clients[newfd].id = ++current_id;
	clients[newfd].msg = NULL;
	sprintf(buffer_msg, "server: client %d just arrived\n", clients[newfd].id);
	send_all(newfd, buffer_msg);
}

void handle_client(int fd)
{
	char buffer[4096];
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes <= 0)
	{
		close(fd);
		FD_CLR(fd, &master);
		sprintf(buffer_msg, "server: client %d just left\n", clients[fd].id);
		send_all(fd, buffer_msg);
		clients[fd].msg = NULL;
		return ;
	}
	buffer[bytes] = '\0';
	clients[fd].msg = str_join(clients[fd].msg, buffer);
	process_messages(fd);
}

void init_server(char *port_number)
{
	struct sockaddr_in servaddr; 
	serverfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (serverfd == -1)
		print_error(NULL);
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(port_number)); 
	if ((bind(serverfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		close(serverfd);
		print_error(NULL);
	}
	if (listen(serverfd, 10) != 0)
	{
		close(serverfd);
		print_error(NULL);
	}
}

void run_server()
{
	maxfds = serverfd;
	FD_ZERO(&master);
	FD_SET(serverfd, &master);
	while (42)
	{
		readfds = master;
		int ret = select(maxfds + 1, &readfds, NULL, NULL, 0);
		if (ret == -1)
			print_error(NULL);
		for (int fd = 0; fd <= maxfds; fd++)
		{
			if (!FD_ISSET(fd, &readfds))
				continue ;
			if (fd == serverfd)
				accept_client();
			else
				handle_client(fd);
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
		print_error("Wrong number of arguments\n");
	init_server(argv[1]);
	run_server();
	return (0);
}