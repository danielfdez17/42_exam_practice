/*
** =============================================================================
** SOLUTION: Array-based (using array of strings for client buffers)
** LANGUAGE: English Comments
** =============================================================================
*/

/*
** mini_serv - Minimalist chat server (Exam Rank 06)
**
** Differences from the provided main.c:
** - Added headers <sys/types.h>, <sys/select.h>, <stdlib.h> and <stdio.h>
**   which are necessary for select(), types like ssize_t, malloc/calloc/free,
**   sprintf and atoi.
** - The original main.c accepted only one connection (direct accept).
**   Here we implement multiplexing with select() for multiple clients.
** - Removed all printf calls from main.c (the server prints nothing during
**   normal execution; errors are handled by print_error(NULL) writing to stderr).
** - The port is no longer hardcoded (8081) and is received via argv[1].
** - sockfd was replaced by the global variable serverfd.
** - connfd and len were removed; connection acceptance happens inside the
**   select() loop.
*/
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>   // Added: types like ssize_t used by recv/select
#include <sys/select.h>  // Added: select(), fd_set, FD_ZERO, FD_SET, FD_CLR, FD_ISSET
#include <stdlib.h>      // Added: malloc, calloc, free, atoi, exit
#include <stdio.h>       // Added: sprintf


/*
** ========================== GLOBAL VARIABLES ==========================
**
** We use arrays indexed by the client's file descriptor (fd).
** Type int[] / char*[]: simple arrays with O(1) access by fd.
** Size 65536: corresponds to the theoretical maximum fd value in Linux
** (defined by /proc/sys/fs/file-max). In practice, select() has
** a limit of FD_SETSIZE (usually 1024), but the larger size
** prevents segfaults if the system allows higher fds.
**
** Alternative: use a struct to group client data.
** Example:
** typedef struct s_client {
**     int     id;
**     char    *msg;
** } t_client;
**
** t_client clients[65536];
**
** Usage:
** clients[fd].id = 1;
** clients[fd].msg = str_join(clients[fd].msg, buffer);
*/

// Maps fd -> sequential client ID.
// Example: ids[7] = 0 means the client with fd 7 received ID 0.
// Type int: IDs are simple sequential integers.
int ids[65536];

// Maps fd -> accumulated message buffer of the client.
// Type char*: pointer to dynamically allocated string by str_join().
// Initialized as NULL; grows as data is received via recv().
char *client_msg[65536];

// Auxiliary buffer to assemble formatted strings with sprintf().
// Type char[4096]: fixed size sufficient for prefixes like
// "server: client X just arrived\n" or "client X: ".
// 4096 is a safe size for any server control message.
char buffer_msg[4096];

// File descriptor of the server socket.
// Type int: standard for POSIX fds. Initialized to -1 (invalid)
// to indicate it hasn't been created yet.
// In the original main.c it was the local variable sockfd.
int serverfd = -1;

// Highest file descriptor in use, used as (maxfds + 1) in select().
// Type int: compatible with the 1st argument of select (nfds).
// Updated with every newly accepted connection.
int maxfds = 0;

// Sequential ID counter. Starts at -1 so the first
// client receives ID 0 after pre-increment (++current_id).
// Type int: simple sequential integers.
int current_id = -1;

// Sets of file descriptors (fd_sets) used by select().
// master: stores the list of ALL currently connected fds (server + clients).
// readfds: temporary copy passed to select(), since select()
// modifies the sets to indicate only the ready fds.
// Kept as globals to facilitate access in the sendAll() function.
fd_set master;
fd_set readfds;

// print_error(): prints an error to stderr and exits the program (exit 1).
// Replaces the multiple printf + exit(0) calls from the original main.c.
// If msg is NULL, prints "Fatal error\n" (system/allocation errors).
void print_error(char *msg)
{
	if (msg)
		write(2, msg, strlen(msg));
	else
		write(2, "Fatal error\n", 12);
	exit(1);
}

/*
** =================== PROVIDED FUNCTIONS (main.c) ====================
** The extract_message() and str_join() functions original to main.c were
** modified (hacked) here to call print_error(NULL) directly
** if malloc/calloc fails. This cleans up the main loop logic.
*/

// extract_message(): extracts the next '\n' terminated message from the buffer.
// Returns 1 if a message was found, 0 if no '\n', -1 on allocation error.
// Parameters:
//   buf (char**): pointer to the client's accumulated buffer (modified in-place).
//   msg (char**): pointer where the extracted message will be stored (includes '\n').
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
				print_error(NULL); // HACK: Original returned (-1). Changed to exit directly.
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

// str_join(): concatenates 'add' to the end of 'buf', freeing the old 'buf'.
// Returns the newly allocated buffer or NULL on failure.
// Parameters:
//   buf (char*): existing buffer (can be NULL on the first call).
//   add (char*): string to be concatenated.
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
		print_error(NULL); // HACK: Original returned (0). Changed to exit directly.
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

/*
** =================== HELPER FUNCTIONS (created) ====================
** Utility functions that didn't exist in the original main.c.
*/


// sendAll(): sends a message to all connected clients,
// except the sender and the server socket.
// Iterates from 0 to maxfds checking the `master` set.
// Parameters:
//   sender (int): fd of the client who sent the message (will be ignored).
//   msg (char*): string to be sent.
// Checks FD_ISSET(fd, &master) to confirm if the client is active.
void sendAll(int sender, char *msg)
{
	int len = strlen(msg);
	for (int fd = 0; fd <= maxfds; fd++)
	{
		if (FD_ISSET(fd, &master) && fd != serverfd && fd != sender)
			send(fd, msg, len, 0);
	}
}

// processMessages(): extracts and forwards complete messages from a client's buffer.
// TCP can fragment or group data, so the buffer might contain zero,
// one, or multiple messages delimited by '\n'.
// Parameters:
//   sender (int): fd of the client whose messages will be processed.
void processMessages(int sender)
{
	// Pointer to store each message extracted by extract_message().
	// Type char*: points to memory internally allocated by extract_message(),
	// which must be freed with free() after use.
	char *msg;

	// Assembles the prefix "client <id>: " in the global buffer.
	// Done once before the loop, since the ID doesn't change between messages.
	sprintf(buffer_msg, "client %d: ", ids[sender]);

	// Type int: extract_message return (1=message, 0=incomplete, -1=error).
	int ret;
	while ((ret = extract_message(&client_msg[sender], &msg)) == 1)
	{
		sendAll(sender, buffer_msg); // Sends the prefix "client X: "
		sendAll(sender, msg);        // Sends the message body (includes '\n')
		free(msg);                   // Frees memory allocated by extract_message()
	}
	// If extract_message returned -1, an allocation failure occurred (calloc).
	if (ret == -1)
		print_error(NULL);
}

/*
** ========================== MAIN FUNCTION ==========================
**
** Changes from the main() in the original main.c:
** - Signature: main() -> main(int argc, char **argv) to receive the port.
** - Removed local variables: sockfd (replaced by global serverfd),
**   connfd and len (accept now occurs inside the select loop).
** - Port: htons(8081) -> htons(atoi(argv[1])) (configurable via argument).
** - Error handling: printf + exit(0) -> print_error(NULL) which writes to stderr
**   and exits with code 1, as required by the subject.
** - Removed cli struct: we pass NULL directly into accept.
** - Added multiplexing logic with select() in the infinite loop.
*/
int main(int argc, char **argv)
{
	// Argument validation (doesn't exist in original main.c).
	// The subject requires exactly 1 argument (the port).
	if (argc != 2)
		print_error("Wrong number of arguments\n");

	// Type struct sockaddr_in: POSIX structure for IPv4 address.
	// Contains family (AF_INET), server IP and port.
	// In the original main.c there was also 'struct sockaddr_in cli' for accept,
	// which was completely removed since we now pass NULL directly to accept().
	struct sockaddr_in servaddr;

	// Creates the TCP socket (SOCK_STREAM) for IPv4 (AF_INET).
	// In main.c it was: sockfd = socket(...) with printf on success/error.
	// Here we use serverfd (global) and handle errors only with print_error(NULL).
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd == -1)
		print_error(NULL);

	// Zeroes out the structure to avoid memory garbage (same as main.c).
	bzero(&servaddr, sizeof(servaddr));

	// Configures the server address (same as main.c, except the port).
	servaddr.sin_family = AF_INET;            // IPv4
	servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1 in network byte order
	// In main.c: htons(8081) hardcoded.
	// Here: htons(atoi(argv[1])) to use the port passed as argument.
	servaddr.sin_port = htons(atoi(argv[1]));

	// Associates the socket to the address/port and puts it in listening mode.
	// In main.c there was a success printf; here we just handle error with print_error(NULL).
	if ((bind(serverfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		close(serverfd); // Optional
		print_error(NULL);
	}
	// listen(fd, backlog): backlog = 10 defines the maximum size of the
	// pending connections queue (same value as original main.c).
	if (listen(serverfd, 10) != 0)
	{
		close(serverfd); // Optional
		print_error(NULL);
	}

	/*
	** select() initialization — completely new section compared to main.c.
	** The original main.c did a single accept() and terminated.
	** Here we use select() to multiplex multiple connections.
	*/
	// serverfd is usually 3 (after stdin=0, stdout=1, stderr=2).
	maxfds = serverfd;
	// FD_ZERO clears all bits in the fd_set, removing any garbage.
	FD_ZERO(&master);
	// Registers the server socket in the set to detect new connections.
	FD_SET(serverfd, &master);

	// Main loop — runs indefinitely until the process is terminated.
	// Everything below is new compared to main.c.
	while (1)
	{
		// Copy of the master fd_sets for this iteration.
		// select() modifies the sets passed, removing fds that
		// are not ready. readfds is recreated from master each iteration.
		readfds = master;
		// select(nfds, readfds, writefds, exceptfds, timeout):
		//   nfds:      maxfds + 1 (highest fd + 1, defines search range)
		//   readfds:   fds monitored for reading (data available or new connection)
		//   writefds:  NULL (ignored in the exam to save code).
		//   exceptfds: NULL (we don't monitor exceptions)
		//   timeout:   NULL (blocks indefinitely until an event occurs)
		// Return: number of ready fds, or -1 on error.
		int ret = select(maxfds + 1, &readfds, NULL, NULL, NULL);
		// select() returning -1 is a fatal error. We don't close FDs manually
		// to save code, trusting the Kernel will do it on exit(1).
		if (ret == -1)
			print_error(NULL);

		// Iterates from 0 to maxfds checking which fds are ready.
		// Unlike epoll (which returns only ready fds), select()
		// requires us to check each fd individually with FD_ISSET.
		for (int fd = 0; fd <= maxfds; fd++)
		{
			// If the fd is not marked in readfds, there is no data to read.
			if (!FD_ISSET(fd, &readfds))
				continue ;

			// ===== NEW CONNECTION =====
			// If the ready fd is the server's, a client wants to connect.
			// In the original main.c, accept() was done once outside a loop.
			// Here, every accept() happens inside the select() loop.
			if (fd == serverfd)
			{
				// accept() requires pointers to store the connecting IP/Port.
				// Since the exam doesn't require knowing this, 'man 2 accept' allows passing NULL.
				// This is superior as we don't create unused variables and code is cleaner.
				/* 
				** Educational alternative (how it would be if we needed the IP):
				** struct sockaddr addr;
				** socklen_t lensock = sizeof(addr);
				** int newfd = accept(serverfd, &addr, &lensock);
				*/
				int newfd = accept(serverfd, NULL, NULL);
				if (newfd == -1)
					continue ; // Provided main.c exits here, but that would kill the whole
					           // server if just one client failed to connect. We simply ignore.

				/*
				** OPTIONAL: next `if`
				** Checks if the fd exceeds FD_SETSIZE (usually 1024).
				** select() uses a fixed-size fd_set; fds >= FD_SETSIZE
				** cause undefined behavior in FD_SET/FD_ISSET.
				** In production, use poll()/epoll() to avoid this limitation.
				*/
				if (newfd >= FD_SETSIZE)
				{
					close(newfd);
					continue;
				}

				FD_SET(newfd, &master);      // Registers for monitoring
				ids[newfd] = ++current_id;   // Assigns sequential ID
				client_msg[newfd] = NULL;    // Initializes empty buffer
				if (newfd > maxfds)          // Updates select() limit
					maxfds = newfd;

				// Notifies other clients about the new connection.
				sprintf(buffer_msg, "server: client %d just arrived\n", ids[newfd]);
				sendAll(newfd, buffer_msg);
				continue ;
			}

			// ===== DATA RECEPTION =====
			// Type char[4096]: local buffer to receive data from recv().
			// Size 4096: common value for network buffers; enough for
			// typical messages without being excessive for the stack.
			char buffer[4096];
			// Type ssize_t: recv() return can be negative (-1 = error).
			// sizeof(buffer) - 1: reserves 1 byte for the terminating '\0'.
			ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

			// ===== DISCONNECTION =====
			// recv() returns 0 when the client closes connection gracefully,
			// or -1 on error. In both cases, we disconnect the client.
			if (bytes <= 0)
			{
				sprintf(buffer_msg, "server: client %d just left\n", ids[fd]);
				sendAll(fd, buffer_msg);     // Notifies others
				FD_CLR(fd, &master);         // Removes from monitoring
				close(fd);                   // Closes the client socket
				// Frees the client's accumulated message buffer.
				// free(NULL) is safe (no-op) in standard C — no guard needed.
				free(client_msg[fd]);
				client_msg[fd] = NULL;
				continue;
			}

			// ===== MESSAGE PROCESSING =====
			// Adds '\0' to treat the received data as a C string.
			buffer[bytes] = '\0';
			// Concatenates to the client's accumulated buffer with str_join().
			// Necessary because TCP can fragment a message across multiple
			// recv() calls, or group several messages in a single recv().
			// extract_message() in processMessages() only extracts full lines.
			client_msg[fd] = str_join(client_msg[fd], buffer);
			if (client_msg[fd] == NULL)
				print_error(NULL);
			processMessages(fd);
		}
	}
	/*
	** Original main.c code (removed):
	** The accept() that existed here was moved inside the select() loop,
	** allowing multiple connections to be accepted over the server's runtime.
	**
	** Removed snippet:
	**   len = sizeof(cli);
	**   connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	**   if (connfd < 0) { printf("server acccept failed...\n"); exit(0); }
	**   else printf("server acccept the client...\n");
	*/
}
