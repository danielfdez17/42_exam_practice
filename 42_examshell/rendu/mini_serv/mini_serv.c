#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * Sends a message to every connected client except one descriptor.
 *
 * The client descriptors are stored in sets[2], which is the writable
 * descriptor set prepared by main before select(). The except descriptor is
 * skipped so that a client does not receive its own connection or message
 * notification. MSG_NOSIGNAL prevents a broken connection from terminating
 * the whole process with SIGPIPE.
 *
 * @param sets   The descriptor sets used by the server. sets[2] must contain
 *               the descriptors that should receive the message.
 * @param except The descriptor that must not receive the message.
 * @param sb     The null-terminated message to broadcast.
 */
void    send_all(fd_set *sets, int except, char *sb)
{
    int fd = -1;
    while (++fd < 1024)
    {
        if (FD_ISSET(fd, &sets[2]) && fd != except)
            send(fd, sb, strlen(sb), MSG_NOSIGNAL);
    }
}

/**
 * Creates, binds, and starts listening on the server socket.
 *
 * This function validates the command-line argument count, allocates one
 * 100000-byte message buffer for each of the 1024 possible descriptors, and
 * configures an IPv4 TCP socket on the loopback address. The port is read
 * from av[1]. On any setup failure it prints an error and terminates the
 * process. On success, the listening socket is placed in sets[0], the master
 * descriptor set used by main.
 *
 * @param ac    The argument count from main.
 * @param av    The argument vector; av[1] is the server port.
 * @param sets  The descriptor-set array shared with the event loop.
 * @param msgp  Output pointer receiving the allocated per-descriptor buffers.
 * @return      The file descriptor of the listening socket.
 */
int create_server(int ac, char **av, fd_set *sets, char (**msgp)[100000])
{
    struct sockaddr_in addr;
    int sock;
    if (ac != 2)
    {
        write(2, "Wrong number of arguments\n", 26);
        exit(1);
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    *msgp = malloc(1024 * sizeof(**msgp));
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(2130706433);
    addr.sin_port = atoi(av[1]);
    if (!*msgp || sock < 0 || bind(sock, (void *)&addr, sizeof(addr)) < 0 || listen(sock, 128) < 0)
    {
        write(2, "Fatal error\n", 12);
        exit(1);
    }
    FD_ZERO(&sets[0]);
    FD_SET(sock, &sets[0]);
    return sock;
}

/**
 * Handles either a new connection or a disconnected client.
 *
 * The listening socket is found by scanning the descriptor set. If fd is the
 * listening socket, accept() creates a client socket, assigns it a unique
 * sequential id, initializes that client's message buffer, adds it to the
 * master set, and broadcasts an arrival notification. Otherwise, fd is an
 * existing client that has disconnected: this function broadcasts a leave
 * notification, removes the descriptor from the master set, and closes it.
 *
 * @param fd   The descriptor reported by select() as requiring attention.
 * @param sets The descriptor sets shared by the event loop.
 * @param id   Per-descriptor storage for the server-assigned client ids.
 * @param msg  Per-descriptor storage for incomplete client messages.
 * @return     Always 0; errors from accept() are treated as no-op events.
 */
int client_event(int fd, fd_set *sets, int *id, char (*msg)[100000])
{
    static int nid;
    char sb[64];
    int sock = 0;
    while (!FD_ISSET(sock, sets))
        sock++;
    if (fd == sock)
    {
        fd = accept(sock, 0, 0);
        if (fd < 0) return 0;
        id[fd] = nid++;
        msg[fd][0] = 0;
        FD_SET(fd, sets);
        sprintf(sb, "server: client %d just arrived\n", id[fd]);
        send_all(sets, fd, sb);
        return 0;
    }
    sprintf(sb, "server: client %d just left\n", id[fd]);
    send_all(sets, fd, sb);
    FD_CLR(fd, sets);
    close(fd);
    return 0;
}

/**
 * Reads and processes data received from one client.
 *
 * Data is appended to the client's persistent buffer because one recv() call
 * may contain only part of a line or several lines. Each newline terminates
 * a message; the completed line is formatted with the client's id and
 * broadcast to all other clients. The buffer is then reused for the next
 * partial line. A return value of zero or less from recv() is treated as a
 * disconnect and delegated to client_event().
 *
 * @param fd   The client descriptor from which data should be read.
 * @param sets The descriptor sets used for broadcasting and cleanup.
 * @param id   Per-descriptor storage for the server-assigned client ids.
 * @param msg  Per-descriptor buffers containing incomplete client messages.
 * @return     Always 0, including when the client is disconnected.
 */
int handle_msg(int fd, fd_set *sets, int *id, char (*msg)[100000])
{
    char sb[100120], rb[65536];
    int ret, i = -1, j;
    ret = recv(fd, rb, sizeof(rb), 0);
    if (ret <= 0) return client_event(fd, sets, id, msg);
    j = strlen(msg[fd]);
    while (++i < ret)
    {
        msg[fd][j] = rb[i];
        if (rb[i] == '\n')
        {
            msg[fd][j] = 0;
            sprintf(sb, "client %d: %s\n", id[fd], msg[fd]);
            send_all(sets, fd, sb);
            j = -1;
        }
        ++j;
    }
    msg[fd][j] = 0;
    return 0;
}

/**
 * Runs the server's select()-based event loop.
 *
 * create_server() initializes the listening socket and the master descriptor
 * set. On each iteration, copies of that set are passed to select(): one is
 * used to find descriptors ready for reading and the other is used by
 * send_all() to find descriptors ready for writing. The first ready
 * descriptor is then dispatched either to client_event() for a new
 * connection or to handle_msg() for client data. The loop continues until
 * the process is terminated externally.
 *
 * @param ac The argument count; the program expects exactly one port.
 * @param av The argument vector containing the port to listen on.
 * @return  Never normally reached; returns 0 if the loop is exited.
 */
int main(int ac, char **av)
{
    fd_set sets[3];
    char (*msg)[100000];
    int id[1024], sock, fd;

    sock = create_server(ac, av, sets, &msg);
    while (1)
    {
        sets[1] = sets[0];
        sets[2] = sets[0];
        if (select(1024, &sets[1], &sets[2], 0, 0) < 0)
            continue;
        fd = -1;
        while (++fd < 1024)
        {
            if (!FD_ISSET(fd, &sets[1]))
                continue;
            if (fd == sock)
                client_event(fd, sets, id, msg);
            else
                handle_msg(fd, sets, id, msg);
            break;
        }
    }
    return 0;
}
