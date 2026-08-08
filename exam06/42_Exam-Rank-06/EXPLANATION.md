# mini_serv / server explanation

This document explains how the provided exercise code (main.c, mini_serv.c, practice.c) implements a simple multi-client chat server handling clients on 127.0.0.1 using `select()`.

## Purpose (subject.en.txt)
- The program listens on 127.0.0.1 at a port given as the first argument.
- It accepts multiple clients and relays messages between them.
- Clients receive arrival and leave notifications: `server: client %d just arrived\n` and `server: client %d just left\n`.
- When a client sends lines terminated by `\n`, each line is forwarded to all *other* clients prefixed with `client %d: `.
- The server must be non-blocking (use `select`) and avoid fd/memory leaks.

## Files overview
- `main.c`: a minimal skeleton / starting code showing socket creation, bind, listen and a single `accept()` call. Not a full server implementation.
- `mini_serv.c`: a complete reference-style implementation that meets the assignment behavior.
- `practice.c`: an equivalent implementation to `mini_serv.c` (slight include/ordering differences and a comment left in `send_all`).

Both `mini_serv.c` and `practice.c` implement the same approach; `main.c` is only a helper/example.

## Key data structures
- `char *bufs[65536];`
  - Per-fd buffer storing data received so far but not yet delivered as complete lines. Index is the client `fd`.
- `int ids[65536];`
  - Per-fd client id assigned when the client connects (0, 1, 2, ...).
- `int next_id` – monotonic counter used to assign ids.
- `fd_set fds_all, fds_read, fds_write;` and `int maxfd;`
  - `fds_all` tracks all fds we care about (listening socket + client sockets).
  - `select()` is called with `fds_read` and `fds_write` copies to discover ready-to-read and ready-to-write sockets.

## Helper functions

### `extract_message(char **buf, char **msg)`
- Purpose: look for the first `\n` in `*buf`. If found:
  - Allocate a new buffer containing the data after that `\n`.
  - Set `*msg` to point to the original buffer, then terminate `*msg` right after the `\n` (`(*msg)[i+1] = 0`).
  - Replace `*buf` with the newly allocated remainder buffer and return `1`.
- Returns:
  - `1` when a full line (message ending with `\n`) was extracted;
  - `0` when no full line exists yet;
  - `-1` on allocation failure.
- Responsibility: the caller owns `*msg` and must `free()` it after use (the code calls `free(msg)` after forwarding).

This function lets the server accumulate partial data from `recv()` calls until a newline-terminated message is complete.

### `str_join(char *buf, char *add)`
- Concatenates `buf` and `add` into a newly allocated buffer, frees the old `buf`, and returns the new buffer pointer.
- Returns `0` on allocation failure.
- Used to append newly-received bytes into the per-fd `bufs[fd]`.

### `send_all(int except, char *msg)`
- Iterates `fd` from `0` to `maxfd` and, for each fd where `FD_ISSET(fd, &fds_write)` and `fd != except`, calls `send(fd, msg, strlen(msg), 0)`.
- `except` is the fd to omit sending to (usually the sender so it doesn't echo to itself).
- The code does not check `send()` return values — this matches the assignment guidance to avoid checking `EAGAIN` and keeps the implementation simple. In practice, ignoring `send()` errors is a trade-off.

### `fatal()`
- Writes `"Fatal error\n"` to stderr and exits with status `1`. Called on unrecoverable errors (socket/bind/listen failures or allocation failures before accept loop).

## Main server loop (behavior)
1. Parse arguments (expect exactly one: the port). On wrong count, print `Wrong number of arguments\n` to stderr and exit(1).
2. Create a listening socket bound to `127.0.0.1` and the given port. On errors before acceptance, call `fatal()`.
3. Initialize `fds_all`, `maxfd = sockfd`, and `FD_SET(sockfd, &fds_all)`.
4. Enter an infinite loop:
   - Copy `fds_all` into `fds_read` and `fds_write` and call `select(maxfd + 1, &fds_read, &fds_write, 0, 0)`.
   - Iterate `fd` from `0` to `maxfd` and handle fds that are set in `fds_read`.

   - If `fd == sockfd` and ready: accept a new connection (`accept(sockfd, 0, 0)`).
     - Assign `ids[newfd] = next_id++`, set `bufs[newfd] = NULL`, `FD_SET(newfd, &fds_all)`, update `maxfd`.
     - Send arrival message `server: client %d just arrived\n` to all other clients via `send_all(newfd, msg)`.

   - Else (an existing client socket is ready to read):
     - `recv(fd, tmp, sizeof(tmp)-1, 0)` into a temporary buffer `tmp`.
     - If `recv()` returned `<= 0`: client disconnected (or error).
       - Announce `server: client %d just left\n` to others with `send_all(fd, msg)`.
       - `FD_CLR(fd, &fds_all)`, `free(bufs[fd])`, set `bufs[fd] = NULL`, and `close(fd)`.
     - Else (`ret > 0`): null-terminate `tmp`, append it to `bufs[fd]` via `str_join`.
       - Repeatedly call `extract_message(&bufs[fd], &msg)`: for each returned message line, format `client %d: %s` and call `send_all(fd, out)` (broadcast to other clients) and `free(msg)`.
       - If `extract_message` returns `-1`, call `fatal()` (allocation error).

## Memory management
- `bufs[fd]` is dynamically allocated and replaced by `extract_message`; when a client disconnects we `free(bufs[fd])` and set it NULL.
- Each extracted `msg` pointer references the old buffer and must be `free()`d after broadcasting — the code does this.
- On allocation failures the server calls `fatal()` (per spec).

## Non-blocking and `select()` details
- The server uses `select()` to multiplex IO. The listening socket and all client sockets are stored in `fds_all` and `select()` discovers read- and write-ready sockets.
- The code checks `fds_write` when deciding which sockets to call `send()` on in `send_all()`. Using the write set avoids calling `send()` on sockets not ready for writing.
- The assignment warns: sockets will block for `recv`/`send` if `select()` wasn't called — that's why `select` is used and why the code only calls `send()` on sockets that `select()` reported writable.

## Differences between the files
- `main.c` is a skeleton showing socket setup and a single `accept()`.
- `mini_serv.c` is the canonical solution and follows the exact behaviors required by the subject.
- `practice.c` is the user's implementation and is functionally the same as `mini_serv.c` (nearly identical logic and function names). It contains a small inline comment `//CHECK THIS ONE!!!` near `send_all` but behaves equivalently.

## How to compile and run
From the repository folder, compile with `gcc` and run the server with a port argument (example `8081`):

```sh
gcc -Wall -Wextra -Werror mini_serv.c -o mini_serv
./mini_serv 8081
# In separate terminals, connect:
# nc 127.0.0.1 8081
# nc 127.0.0.1 8081
```

Type messages followed by `Enter` in one client; each newline-delimited line is forwarded to the other connected clients prefixed with `client <id>:`. When a client connects or disconnects, all other clients receive the corresponding server message.

## Notes / caveats
- The code avoids complicated partial-write handling: it trusts `select()`'s write readiness and ignores `send()` return values. This matches the assignment constraints that instruct not to check `EAGAIN` and to rely on `select()`.
- The `out` buffer in the implementation is large (120000 bytes) to accommodate long messages; ensure stack limits are respected when tweaking that size.
- The arrays sized `65536` assume file descriptors fall within that range (typical on Unix). If running in an environment with very large fd values, this approach would need adjustment.

---
If you want, I can:
- Run a quick static check or compile `mini_serv.c` here and report any warnings/errors.
- Add short inline comments to `practice.c` pointing to where each behavior is implemented.
