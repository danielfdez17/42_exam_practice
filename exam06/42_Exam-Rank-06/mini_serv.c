#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>

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
				return (-1);
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
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}
char	*bufs[65536];
int		ids[65536];
int		next_id = 0;

fd_set	fds_read, fds_write, fds_all;
int		maxfd = 0;

void	send_all(int except, char *msg)
{
	for (int fd = 0; fd <= maxfd; fd++)
		if (FD_ISSET(fd, &fds_write) && fd != except)
			send(fd, msg, strlen(msg), 0);
}

void	fatal(void)
{
	write(2, "Fatal error\n", 12);
	exit(1);
}

int main(int argc, char **argv) {
	if (argc != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}
	int sockfd;
	struct sockaddr_in servaddr; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) fatal(); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(argv[1])); 
  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) fatal();
	if (listen(sockfd, 100) != 0) fatal();

	FD_ZERO(&fds_all);
	FD_SET(sockfd, &fds_all);
	maxfd = sockfd;

	while(1) {
		fds_read = fds_write = fds_all;
		if (select(maxfd +1, &fds_read, &fds_write, 0, 0) < 0) continue;

		for(int fd = 0; fd <= maxfd; fd++) {
			if (!FD_ISSET(fd, &fds_read)) continue;

			if (fd == sockfd) {
				int newfd = accept(sockfd, 0, 0);
				if (newfd < 0) continue;
				ids[newfd] = next_id++;
				bufs[newfd] = NULL;
				FD_SET(newfd, &fds_all);
				if (newfd > maxfd) maxfd = newfd;

				char msg[64];
				sprintf(msg, "server: client %d just arrived\n", ids[newfd]);
				send_all(newfd, msg);
			}
			else {
				char tmp[4096];
				int ret = recv(fd, tmp, sizeof(tmp) -1, 0);

				if (ret <= 0) {
					char msg[64];
					sprintf(msg, "server: client %d just left\n", ids[fd]);
					send_all(fd, msg);
					FD_CLR(fd, &fds_all);
					free(bufs[fd]);
					bufs[fd] = NULL;
					close(fd);
				}
				else {
					tmp[ret] = 0;
					bufs[fd] = str_join(bufs[fd], tmp);
					if (!bufs[fd]) fatal();

					char *msg;
					int ret2;
					while((ret2 = extract_message(&bufs[fd], &msg)) == 1) {
						char out[120000];
						sprintf(out, "client %d: %s", ids[fd], msg);
						send_all(fd, out);
						free(msg);
					}
					if (ret2 == -1) fatal();
				}
			}
		}
	}
	return(0);
}