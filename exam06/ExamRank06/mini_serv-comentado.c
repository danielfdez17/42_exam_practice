/* Este programa crea un servidor TCP que escucha solo en 127.0.0.1 
y en el puerto pasado por argumento, y usa select para poder gestionar 
muchos clientes a la vez sin bloquearse: cuando select indica que el socket 
del servidor está listo, hace accept y añade al nuevo cliente, 
le asigna un id incremental y avisa al resto con 
"server: client %d just arrived\n"; 
cuando select indica que un cliente tiene datos, hace recv, 
va acumulando el texto hasta encontrar saltos de línea \n y, 
por cada línea completa, la reenvía a todos los demás con el prefijo "client %d: ";
si recv devuelve 0 o error, considera que el cliente se fue, 
avisa al resto con "server: client %d just left\n", 
lo elimina del set con FD_CLR y cierra su fd. */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Sirve para guardar lo recibido hasta poder “cortar” por \n 
// y mandar líneas completas con prefijo
typedef struct s_client
{
	int id; // id: el identificador que pide el subject (0,1,2,...)
	char msg[370000]; // buffer acumulador por cliente
}	t_client;

/* clients[2048]
Indexa por fd (file descriptor)
clients[fd] guarda el estado de ese cliente.
2048 suele ser suficiente para el límite de fds 
en el tester (y el subject suele usar ese estilo). */

t_client clients[2048];
// current: conjunto “maestro” de fds que estás vigilando (server socket + clientes)
/* read_set, write_set: copias temporales que pasas a select en cada vuelta.
Importante: select modifica los sets, 
por eso hay que copiar desde current cada iteración.*/
fd_set read_set, write_set, current;
/* ¿Qué es fd_set?
Es un tipo (estructura/bitset interno) que representa un conjunto de fds.
Se usa con select para decir:
“vigila estos fds para lectura”
“vigila estos para escritura” */

/* maxfd: Máximo fd dentro de current.
select(nfds, ...) necesita maxfd + 1.
gid: “global id”: contador para asignar ids consecutivos a clientes*/

int maxfd = 0, gid = 0;
/* send_buffer y recv_buffer
recv_buffer: lo que llega del recv.
send_buffer: el mensaje ya formateado que quieres reenviar 
(“server: …” o “client X: …”) */
char send_buffer[400000],recv_buffer[400000];

/* ¿Qué es send_buffer?
Un array de char donde construyes el mensaje que vas a reenviar, por ejemplo:
"server: client 3 just arrived\n"
"client 2: hola\n"
Se rellena con sprintf(...) y luego send_to_all lo usa para mandarlo. */

void err(char *msg)
{
	if(msg)
		write(2,msg,strlen(msg));
	else
		write(2,"Fatal error",11);
	write(2,"\n",1);
	exit(1);
}

/* ¿Qué es except?
Es el fd al que NO quieres enviarle el mensaje.
Si un cliente manda un mensaje, no quieres devolvérselo a él mismo.
Si un cliente llega/se va, según enunciado, el mensaje se manda a “los demás”.
En send_to_all(except):
manda a todos menos al except. */

void send_to_all(int except)
{
	for(int fd = 0; fd <= maxfd; fd++)
	{
		/* ¿Qué es FD_ISSET?
Macro que responde: ¿este fd está dentro del set?
Antes de select: lo usas menos.
Después de select: significa “¿este fd está listo?” */

		// Significa: “si fd tiene algo para leer (o un accept pendiente)”
		if(FD_ISSET(fd,&write_set) && fd != except)
		{
			/* send(fd, buffer, size, flags) manda bytes por un socket.
			Devuelve cuántos bytes mandó, o -1 si error. 
			Manda el texto que hay en send_buffer al socket fd.*/
			if(send(fd,send_buffer,strlen(send_buffer),0) == -1)
				err(NULL);
		}
	}
}

/* ¿Qué es write_set?
Es el conjunto de fds que select te marca como “listos para escribir”.
Si FD_ISSET(fd, &write_set) es true, normalmente send(fd,...) no bloqueará. */

int main(int argc, char **argv) {
	if (argc != 2)
		err("Wrong number of arguments");
	int sockfd;
	struct sockaddr_in servaddr;
       	socklen_t len = sizeof(struct sockaddr);

	/* ¿Qué es struct sockaddr_in servaddr?
sockaddr_in es una estructura específica para direcciones IPv4 (AF_INET). Contiene:
-familia (AF_INET)
-IP
-puerto
Dentro tiene campos como:
-sin_family
-sin_addr.s_addr
-sin_port */

	/* ¿Qué es socklen_t len?-
socklen_t es el tipo correcto para tamaños de estructuras de direcciones en sockets (lo pide la API).
Se usa sobre todo en accept, getsockname, etc.
socklen_t len = sizeof(struct sockaddr);
Esto NO define struct sockaddr.
Lo que hace es:
-usar el tipo ya existente struct sockaddr (lo declara el sistema en headers)
-y calcula su tamaño con sizeof(...)
O sea: lo está usando, no definiendo.
Nota: aquí sería más típico sizeof(struct sockaddr_in) o usar una variable struct sockaddr_in cli; len = sizeof(cli);. Pero para el tester normalmente da igual. */

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* AF_INET y SOCK_STREAM
	¿Qué es AF_INET?
	Significa: “Address Family: Internet (IPv4)”.
	Indica que usarás direcciones IPv4 (sockaddr_in).
	
	¿Qué es SOCK_STREAM?
	Significa: socket de tipo “stream” → TCP.
	Conexión fiable, ordenada.
	Es el típico para chat servers. */

	if (sockfd == -1)
	       err(NULL);
	maxfd = sockfd;
	bzero(&servaddr, sizeof(servaddr)); 
	bzero(clients,sizeof(clients));
	FD_ZERO(&current); // Macro que vacía un fd_set. vacía la lista
	FD_SET(sockfd,&current); // añade el socket del servidor a esa lista

	/* FD_SET(sockfd, &current); mete el fd sockfd dentro del conjunto (fd_set) llamado current.
	current es “la lista de fds que el servidor está vigilando con select” */

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; // “Esta dirección es IPv4”
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	/* sin_addr.s_addr es la IP en formato numérico.
	2130706433 es 127.0.0.1 en entero (0x7F000001).
	htonl = “host to network long”: convierte el entero al orden de bytes de red (big-endian).
	Esto se hace porque la red usa un orden estándar. */
	
	servaddr.sin_port = htons(atoi(argv[1])); 
  
	/* ¿Qué es bind?
	bind(sockfd, addr, addrlen) “pega” tu socket a una IP y puerto concretos.
	Traducción:
	“Este socket va a escuchar en 127.0.0.1:PUERTO”.
	Sin bind, el socket no sabe “dónde” escucha. */

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) == -1) 
	    err(NULL);
	
	/* bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))
	sockfd: tu socket del servidor.
	(struct sockaddr *)&servaddr: la API de bind espera un puntero genérico struct sockaddr*, así que haces cast desde sockaddr_in*.
	sizeof(servaddr): tamaño de la estructura que le pasas.
	Si bind devuelve -1, hubo error (puerto ocupado, permisos, etc.) → “Fatal error”. */

	/* ¿Qué es listen?
	Pone el socket en modo “servidor”:
	ya no es para conectar tú, sino para aceptar conexiones.
	crea una cola de conexiones pendientes.
	listen(sockfd, 100):
	100 = tamaño de la cola (backlog).
	Si llegan más de 100 a la vez, pueden fallar o esperar según OS. */

	if (listen(sockfd, 100) == -1)
		err(NULL);

	/* listen(sockfd, 100)
	Activa el modo servidor y la cola de pendientes (backlog 100).
	Si falla → fatal. */

	while(1)
	{
		// O sea: vigilan los mismos fds para lectura y para escritura
		read_set = write_set = current;
		if(select(maxfd + 1,&read_set,&write_set,0,0) == -1)
			continue;
		for (int fd = 0; fd <= maxfd; fd++)
		{
			if(FD_ISSET(fd,&read_set)) // preguntas FD_ISSET(fd, &read_set) para saber cuáles están listos
			{
				if(sockfd == fd)
				{
					/* ¿Qué es accept?
				accept(sockfd, &addr, &len) toma una conexión entrante de la cola de listen y te devuelve un nuevo fd para hablar con ese cliente.
				Muy importante:
				sockfd se queda escuchando para futuras conexiones.
				accept devuelve clientfd (otro fd) para hablar con ese cliente. */
					int clientfd = accept(fd,(struct sockaddr *)&servaddr,&len);
					if (clientfd == -1)
						continue;
					if (clientfd > maxfd)
						maxfd = clientfd;
					clients[clientfd].id = gid++;
					FD_SET(clientfd,&current);
					sprintf(send_buffer,"server: client %d just arrived\n",clients[clientfd].id);
					send_to_all(clientfd);
					break ;
				}
				else
				{
					/* 
				recv(fd, buffer, size, flags) lee bytes desde un socket.
				Devuelve un número ret:
				ret > 0: recibió ret bytes (se guardaron en buffer).
				ret == 0: el cliente cerró la conexión (disconnect limpio).
				ret < 0: error. 
				Lee del socket del cliente fd y mete lo recibido en recv_buffer*/
					int ret = recv(fd,recv_buffer,sizeof(recv_buffer),0);
					if (ret <= 0)
					{
						sprintf(send_buffer,"server: client %d just left\n",clients[fd].id);
						send_to_all(fd);
						FD_CLR(fd,&current); // quita el fd fd del conjunto current. Deja de vigilar ese socket en select
						bzero(clients[fd].msg,strlen(clients[fd].msg));
						close(fd); // Luego lo cierras con close(fd)
						break ;
						/* Si no haces FD_CLR y cierras el fd:
					select podría seguir viendo ese número de fd como “activo” 
					(o peor, el sistema podría reutilizar ese número para otra cosa) */
					}
					else
					{
						for(int i = 0, j = strlen(clients[fd].msg); i < ret; i++, j++)
						{
							clients[fd].msg[j] = recv_buffer[i];
							if(clients[fd].msg[j] == '\n')
							{
								clients[fd].msg[j] = '\0';
								sprintf(send_buffer,"client %d: %s\n",clients[fd].id,clients[fd].msg);
								send_to_all(fd);
								bzero(clients[fd].msg,strlen(clients[fd].msg));
								j = -1;
							}
						}
					}
				}
			}
		}
	}
}

/* socket() → crea “enchufe”

bind() → le pone dirección: 127.0.0.1:PUERTO

listen() → “me pongo a escuchar”

select() → “espero eventos sin bloquear”

accept() → “entra alguien, le doy un clientfd”

recv() → “leo lo que me dice un cliente”

send() → “se lo mando a otros clientes” */

/* 
FD_SET = añadir fd al set

FD_CLR = quitar fd del set

FD_ISSET = comprobar si está en el set (o si select lo marcó listo)

FD_ZERO = vaciar el set */