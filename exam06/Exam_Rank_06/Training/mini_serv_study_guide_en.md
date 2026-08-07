# Mini_serv Study Guide (Struct Version)

This guide breaks down the [en/struct/mini_serv.c](en/struct/mini_serv.c) implementation for the Rank 06 exam. It incorporates the strict requirements from the [subject.en.txt](../subject/subject.en.txt) and utilizes the typically provided [main.c](../subject/main.c).

> [!TIP]
> This guide focuses on the **struct approach** (`en/struct/mini_serv.c`). The array approach (`en/array/mini_serv.c`) is also covered for reference, but the struct version is recommended for better code organization. The logic is **identical** — only data access differs.

---

## 1. Core Theory & Fundamentals

Network application development in Linux is based on the Sockets API. The UNIX model dictates that "almost everything is a file". Therefore, to perform I/O on sockets, we use system calls similar to those for regular files (like `read` and `write`). Each connection is represented by a file descriptor (fd), which is just a small non-negative integer.

**The Concurrency Challenge:**
A chat server needs to handle multiple clients simultaneously. Using blocking calls (like a `recv` waiting for data from a single client) would prevent the server from serving other users. To solve this without using threads (which are forbidden in the context of mini_serv), we use the technique of **I/O multiplexing** with the `select()` function.

---

## 2. Exam Rules & Requirements (Crucial)

### Allowed Functions
`write`, `close`, `select`, `socket`, `accept`, `listen`, `send`, `recv`, `bind`, `strstr`, `malloc`, `realloc`, `free`, `calloc`, `bzero`, `atoi`, `sprintf`, `strlen`, `exit`, `strcpy`, `strcat`, `memset`.
> **WARNING:** `printf` is **FORBIDDEN**. You must use `write` for everything (or `send` for sockets). Also **NO `#define`** preprocessor directives.

### Strict Output Messages
The exam tester checks these exact strings. A typo = fail.
1.  **Argument Error**: `Wrong number of arguments\n` → stderr, exit 1
2.  **System/Malloc Error**: `Fatal error\n` → stderr, exit 1
3.  **Client Arrival**: `server: client %d just arrived\n`
4.  **Client Left**: `server: client %d just left\n`
5.  **Message Prefix**: `client %d: ` → prepended to **every line** of the message

> [!CAUTION]
> The message format is `client %d: ` (with a space after the colon). A single missing space or wrong newline will fail the diff check.

### Network & Behavior
-   **IP**: Must bind to `127.0.0.1` (hardcoded as `htonl(2130706433)`).
-   **Port**: Passed as the first argument (`argv[1]`).
-   **Non-blocking**: Program must be non-blocking (handled via `select`), but **DO NOT** disconnect "lazy" clients.
-   **IDs**: Client IDs start at 0 and increment indefinitely (never reused).
-   **Speed**: Send messages as fast as possible — don't buffer unnecessarily.

---

## 3. Provided Code Analysis ([main.c](../subject/main.c))

The exam provides a `main.c` file with a basic structure. You must adapt this file based on the rules and multiplexing logic.

### What to KEEP from main.c:
1. **[extract_message](../subject/main.c#L8-L33)**: Essential for TCP (which is a stream protocol). Splits the buffer into complete lines ending in `\n`.
2. **[str_join](../subject/main.c#L35-L53)**: Used to accumulate partial data. **Warning:** This function automatically calls `free()` on the pointer passed as its first argument!
3. **Socket Boilerplate**: The socket creation, `sockaddr_in` setup (IP 127.0.0.1 and port), `bind()`, and `listen()`.

### What to CHANGE or REMOVE:
1. **Remove `printf`**: It is strictly forbidden. Replace with `write` (to stderr for errors) or `send` for network communication.
2. **Replace the single `accept()`**: The original code only accepts one client. Replace it with an infinite `while(1)` loop containing the `select()` event loop.
3. **Implement Client Management**: You will need to manage buffers and IDs indexed by the client's `fd` using a struct (such as `t_client clients[FD_SETSIZE]`).

---

## 4. Core Concepts

### Multiplexing with `select()`
The core challenge is handling multiple clients without threading. `select()` allows the program to "watch" multiple file descriptors (sockets) at once and wake up only when one of them is ready.

| Macro/Function | Purpose |
|---|---|
| `FD_ZERO(&set)` | Clears the entire set |
| `FD_SET(fd, &set)` | Adds `fd` to the set |
| `FD_CLR(fd, &set)` | Removes `fd` from the set |
| `FD_ISSET(fd, &set)` | Checks if `fd` is in the set (used **after** `select` returns) |
| `select(maxfd+1, &read, NULL, NULL, NULL)` | Blocks until activity occurs |

> [!WARNING]
> **Golden Rule: `select()` is destructive!**
> It modifies the sets you pass to it to indicate who is ready. You must **always** keep an intact master set (`master`) and copy its values to the working set (`readfds`) at the **start** of every main loop iteration.

### Buffer Management
TCP is a **stream** protocol, not a packet protocol:
- You might receive a **partial** message (e.g., `"Hel"`)
- You might receive **multiple** messages at once (e.g., `"Hello\nWorld\n"`)
- **Solution**: Accumulate received data into a per-client buffer and extract complete lines ending in `\n` using [extract_message](../subject/main.c#L8-L33).

### POSIX Type Corrections (Compared to main.c)
The provided `main.c` has some type inaccuracies that cause compilation warnings or vulnerabilities. Our solution fixes these:

1. **`accept` with `NULL` parameters (Total Simplification)**:
   The original `main.c` creates variables (`int len` and `struct sockaddr_in cli`) to receive the IP and port of whoever connected via `accept()`. But our chat server doesn't need this information! According to `man 2 accept`, if you don't need the client's address, you can pass `NULL` pointers. We replaced all that allocation simply with `accept(serverfd, NULL, NULL)`.
   **Why is it better?** You save time in the exam (less typing), eliminate unused variables, and as a bonus, you escape the problem of having to correct the type `int len` to `socklen_t` (which causes compilation warnings in 64-bit). As a curiosity, the purist/educational way if you *did* need the IP would be to declare `struct sockaddr addr; socklen_t lensock = sizeof(addr);` and pass their addresses.
2. **`recv` return stored in `ssize_t`**:
   `recv()` returns the number of bytes read or `-1` on error. The `ssize_t` (Signed Size) type is designed exactly for this, accommodating the architecture's maximum memory limit without the risk of *overflow* (unlike a regular `int`).

> **Where to read more about Address Structs:**
> The `accept` and `recv` manuals don't detail these structures deeply because they depend on the network layer (IPv4, IPv6, Unix Sockets, etc.). To understand their hierarchy, check:
> * **`man 7 socket`**: Explains the general API and the generic `sockaddr` struct.
> * **`man 7 ip`**: Explains the specific IPv4 implementation and `sockaddr_in`.
> * **Beej's Guide to Network Programming:** Section *3. IP Addresses, structs, and Data Munging* is the best didactic explanation available on how C handles this "salad" of structs inherited from the 80s.
> * **The Linux Programming Interface (TLPI):** By Michael Kerrisk. The definitive book on the Linux system interface. Chapters 56 through 61 dive deep into the theoretical and practical foundations of Sockets and I/O Multiplexing.

---

## 5. Data Structures

### Struct Approach (Primary — `en/struct/mini_serv.c`)
Uses a struct for clean data cohesion:
```c
typedef struct s_client {
    int   id;    // Sequential client ID
    char  *msg;  // Accumulated message buffer
} t_client;

t_client clients[FD_SETSIZE]; // Indexed by fd
```
**Access**: `clients[fd].id` and `clients[fd].msg`

**Advantages:**
- Data about one client is grouped together
- Easier to add more fields later (e.g., nickname)
- Cleaner, self-documenting code

### Array Approach (Alternative — `en/array/mini_serv.c`)
Uses parallel arrays:
```c
int   ids[65536];          // fd → client ID
char  *client_msg[65536];  // fd → message buffer
```
**Access**: `ids[fd]` and `client_msg[fd]`

**Advantages:**
- Slightly less typing for a speed exam
- Conceptually simpler

> Both approaches use the same logic and both have the `if (newfd >= FD_SETSIZE)` guard that prevents undefined behavior in `FD_SET`. The struct version sizes its array to `FD_SETSIZE` (typically 1024) since no fd above that will ever be stored, while the array version uses 65536 for extra safety on array access — but the real UB protection comes from the guard, not the array size. Either works for the exam.

### Global Variables (Common to Both)
```c
char   buffer_msg[128];  // Scratch buffer for sprintf (server messages)
int    serverfd = -1;    // Listening socket fd
int    maxfds = 0;       // Highest fd tracked (for select's first arg)
int    current_id = -1;  // Counter; starts at -1 so first client gets ID 0 via ++

fd_set master;    // Persistent set of ALL active fds
fd_set readfds;   // Temporary copy for select() — who has data to read?
```

---

## 6. Code Breakdown (Struct Version)

### Error Handling — [print_error](en/struct/mini_serv.c#L161-L168)
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
- Dual-purpose: pass a specific message (`"Wrong number of arguments\n"`) or `NULL` for the generic fatal error.
- Always writes to **stderr** (fd 2) and exits with code **1**.

### Broadcasting — [sendAll](en/struct/mini_serv.c#L180-L188)
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
> **Why do we check `&master` instead of `&writefds`?**
> In the real world, calling `send()` when a client's TCP buffer is full will cause your server to block (freeze). Using `writefds` in `select()` prevents this.
> However, in the `mini_serv` exam, tests run locally with tiny strings, and Moulinette clients read data instantly. The buffer never fills.
> Since `send()` will never block during the exam, removing the `writefds` boilerplate cuts several lines of code!

Three conditions filter who receives the message:
1. **`FD_ISSET(fd, &master)`** — Confirms the client exists in the network and is valid
2. **`fd != serverfd`** — Don't send to the listening socket
3. **`fd != sender`** — Don't echo back to the sender

> [!NOTE]
> **Why new clients don't receive their own "just arrived" message:**
> The `sendAll` function skips the sender via the `fd != sender` condition. Since we pass the new client's `fd` as the sender of the "just arrived" message, it is naturally filtered out.

### Message Processing — [processMessages](en/struct/mini_serv.c#L198-L220)
```c
void processMessages(int sender)
{
    char *msg;
    sprintf(buffer_msg, "client %d: ", clients[sender].id);

    while (extract_message(&clients[sender].msg, &msg) == 1)
    {
        sendAll(sender, buffer_msg);  // Send prefix "client X: "
        sendAll(sender, msg);         // Send the line (includes '\n')
        free(msg);                    // Free memory from extract_message
    }
}
```
Key points:
- `sprintf` builds the prefix **once** before the loop (ID doesn't change)
- Two separate `sendAll` calls: one for the prefix, one for the line content
- The `while` loop handles the case of multiple `\n` in one recv

### Server Init — [init_server](en/struct/mini_serv.c#L233-L266)
Extracted from main.c boilerplate — `socket()` → `bzero()` → configure address → `bind()` → `listen()`. Only differences from main.c:
- Port from `argv[1]` instead of hardcoded `8081`
- Errors call `print_error(NULL)` instead of `printf`

### Accepting Clients — [accept_client](en/struct/mini_serv.c#L273-L310)
```c
void accept_client()
{
    int newfd = accept(serverfd, NULL, NULL);
    if (newfd == -1)
        return ; // The original main.c uses exit(), but an accept() error shouldn't terminate the server.

    if (newfd >= FD_SETSIZE)       // Safety: select() can't handle fds >= 1024
    {
        close(newfd);
        return ;
    }

    FD_SET(newfd, &master);          // Add to watch list
    clients[newfd].id = ++current_id; // Assign sequential ID
    clients[newfd].msg = NULL;        // Init empty buffer
    if (newfd > maxfds)              // Update select() range
        maxfds = newfd;

    sprintf(buffer_msg, "server: client %d just arrived\n", clients[newfd].id);
    sendAll(newfd, buffer_msg);
}
```

> [!WARNING]
> The `FD_SETSIZE` guard is important! `FD_SET` with an fd ≥ `FD_SETSIZE` causes **undefined behavior**. In the exam you may omit it if pressed for time, but it's good practice.

### Handling Client Data — [handle_client](en/struct/mini_serv.c#L319-L359)
```c
void handle_client(int fd)
{
    char buffer[4096];
    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)                          // Disconnect or error
    {
        sprintf(buffer_msg, "server: client %d just left\n", clients[fd].id);
        sendAll(fd, buffer_msg);
        free(clients[fd].msg);                   // free(NULL) is safe — no guard needed
        clients[fd].msg = NULL;                  // Prevents garbage if fd is recycled
        FD_CLR(fd, &master);
        close(fd);
        return;
    }
    buffer[bytes] = '\0';                    // Null-terminate for str_join
    clients[fd].msg = str_join(clients[fd].msg, buffer);
    processMessages(fd);
}
```

### Main Event Loop — [run_server](en/struct/mini_serv.c#L367-L421)
```c
void run_server()
{
    maxfds = serverfd;
    FD_ZERO(&master);
    FD_SET(serverfd, &master);

    while (1)
    {
        readfds = master;       // Copy! select() will modify these

        int ret = select(maxfds + 1, &readfds, NULL, NULL, NULL);
        if (ret == -1)          // Fatal — can't recover from broken select
            print_error(NULL);  // Note: The proper way is to close all FDs before exiting. We omit this
                                // cleanup loop to save time in the exam (it's not tested by Moulinette).

        for (int fd = 0; fd <= maxfds; fd++)
        {
            if (!FD_ISSET(fd, &readfds))   // No pending data → skip
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
> **Why `select() == -1` is fatal (not just `continue`):**
> Without a successful `select()`, the `readfds` set contains garbage. A `continue` would loop back, copy `master` again, but if the underlying issue persists (e.g., bad fd in master), it would busy-loop forever. The subject says system call errors before connections = fatal error.

> [!IMPORTANT]
> **Why `FD_ISSET(fd, &readfds)` is essential:**
> Without it, we'd try `recv()` on fds that have no pending data (could block) or `accept()` on the server fd without a pending connection. `select()` marks in `readfds` **ONLY** the fds that have activity — skipping this check would break the entire multiplexing logic.

---

## Sockets and Multiplexing API Reference (System Calls)

This is a quick cheat sheet of the functions you will use in the exam, detailing their arguments:

### `int socket(int domain, int type, int protocol)`
Creates a communication endpoint (socket) and returns its File Descriptor.
*   **`domain`**: The protocol family. We use `AF_INET` for IPv4.
*   **`type`**: The communication type. We use `SOCK_STREAM` for reliable, connection-oriented TCP streams.
*   **`protocol`**: The specific protocol. We use `0` to let the system automatically choose the default protocol for the given `type` (which will be TCP).

### `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`
Assigns (binds) the newly created socket to a specific local IP address and Port.
*   **`sockfd`**: The File Descriptor returned by `socket()`.
*   **`addr`**: A pointer to the generic `struct sockaddr`. We fill a `struct sockaddr_in` with the IP and Port, and cast it using `(const struct sockaddr *)`.
*   **`addrlen`**: The size in bytes of the address structure (e.g., `sizeof(servaddr)`).

### `int listen(int sockfd, int backlog)`
Marks the socket as passive, meaning it will be used to "listen" for and accept incoming connections.
*   **`sockfd`**: The server's File Descriptor.
*   **`backlog`**: The maximum length of the queue of pending connections. We use `10` (inherited from the boilerplate). If more clients try to connect simultaneously, they will be rejected.

### `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)`
Accepts the first connection waiting in the `listen` queue, creating a *new* dedicated socket to communicate with that client.
*   **`sockfd`**: The passive listening server socket.
*   **`addr`**: A pointer where the system would write the IP/Port of the newly connected client. We pass `NULL` since we don't need this info for the exam.
*   **`addrlen`**: A pointer to the size of `addr`. We pass `NULL`.
*   **Return**: A **new** File Descriptor used solely to talk to that specific client.

### `int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)`
The core multiplexing function. Pauses the program and waits until at least one of the sockets is "ready" to read or write.
*   **`nfds`**: The number of the highest File Descriptor being monitored **plus one** (`maxfds + 1`).
*   **`readfds`**: Set of fds you want to monitor to see if there is **new data to read** (or new incoming connections for the `serverfd`).
*   **`writefds`**: Set of fds you want to monitor to see if there is **free space to write**. In the exam, we **pass `NULL`** to simplify the code, because Moulinette never simulates slow clients that would block sending.
*   **`exceptfds`**: Set to monitor for exceptional errors. We pass `NULL` (we don't care).
*   **`timeout`**: Maximum time for select to wait. We pass `NULL` to make it block indefinitely until something happens.

### `ssize_t recv(int sockfd, void *buf, size_t len, int flags)`
Reads data (messages) sent by the client.
*   **`sockfd`**: The specific client's fd.
*   **`buf`**: The buffer (char array) where the received data will be stored.
*   **`len`**: The maximum number of bytes to read. We pass `sizeof(buffer) - 1` to guarantee space for the `\0` terminator.
*   **`flags`**: Special modifiers. We use `0` (no modifiers).
*   **Return**: The number of bytes read. If it returns `0`, it means the client disconnected (EOF). If `< 0`, error.

### `ssize_t send(int sockfd, const void *buf, size_t len, int flags)`
Sends data (messages) to a client.
*   **`sockfd`**: The destination client's fd.
*   **`buf`**: The string/data you want to send.
*   **`len`**: The number of bytes to send (usually `strlen(buf)`).
*   **`flags`**: Modifiers. We use `0`.

### `fd_set` Macros (For use with `select`)
An `fd_set` is just a bit array under the hood. These macros manipulate it:
*   **`FD_ZERO(&set)`**: Clears the entire set. Essential for initializing `master` and removing memory garbage.
*   **`FD_SET(fd, &set)`**: Turns on the bit for `fd` in the set (adds it to monitoring).
*   **`FD_CLR(fd, &set)`**: Turns off the bit for `fd` (removes it from monitoring).
*   **`FD_ISSET(fd, &set)`**: Asks if the bit for `fd` is turned on. `select` uses this to tell us which clients are ready!
*   **`FD_SETSIZE`**: Not a function, but a native Linux constant (usually 1024) that defines the bit size of the `fd_set` struct and dictates the absolute limit of file descriptors that `select()` can handle at once.

### Other Useful LibC Functions
*   **`void bzero(void *s, size_t n)`**: Fills a memory block with zeros. In `mini_serv`, we use it to "clean" the `servaddr` struct before configuring it. This is crucial because stack-allocated structs start with memory garbage, which would cause `bind()` to fail. (Tip: it's the legacy version of `memset`).
*   **`int sprintf(char *str, const char *format, ...)`**: Works just like `printf`, but instead of printing to the screen, it "prints" (writes) the formatted text into a string (`char` array). It's the easiest and fastest way to inject the client's `id` (int) into the `"server: client %d just arrived\n"` phrase without having to code an `itoa` by hand.

---

## 7. Key Helper Functions (From Provided [main.c](../subject/main.c))

### [int extract_message(char \*\*buf, char \*\*msg)](../subject/main.c#L8-L33)
-   **Provided**: Yes — but we will modify it to crash on memory allocation errors.
-   **Role**: Scans `*buf` for a `\n`.
-   **Returns**:
    | Value | Meaning |
    |---|---|
    | `1` | Found a line → copied to `*msg` (including `\n`), remainder stays in `*buf` |
    | `0` | No complete line yet (partial data or empty) |
    | `-1` | `calloc` failed → you must handle as fatal error |
-   **Tip**: Always call in a `while` loop: `while (extract_message(...) == 1)`

### [char \*str_join(char \*buf, char \*add)](../subject/main.c#L35-L53)
-   **Provided**: Yes — but we will modify it to crash on memory allocation errors.
-   **Role**: Concatenates `add` to `buf`.
-   **CRITICAL BEHAVIOR**: It calls `free(buf)` internally!
-   **Usage**: `msg = str_join(msg, buffer);` — the old `msg` pointer is **invalid** after this call.
-   **Returns NULL** on malloc failure → check and handle as fatal error.

---

## 8. Exam Tips & Pitfalls

### Memory Leaks (most common failure)
-   Always `free(clients[fd].msg)` and set to `NULL` when a client disconnects.
-   Always `free(msg)` inside the `processMessages` loop after sending.
-   `str_join` already frees `buf` — don't double-free!

### `select` Destructiveness
-   **Never** pass `&master` directly to `select`. You **must** copy it to `readfds` every iteration.

### Max FD Tracking
-   `select`'s first argument is `maxfds + 1`. Update `maxfds` on every `accept`.
-   Note: we don't decrement `maxfds` on disconnect (slight inefficiency, but correct and simpler).

### `write` vs `send`
-   Both work for sockets. `send` is socket-specific and technically more correct.
-   For stderr messages, you must use `write` (not a socket).

### Return Value Checks
-   Check `malloc`/`calloc`, `socket`, `bind`, `listen`, `select`, `accept` for errors.
-   The subject requires `Fatal error\n` for all system call failures.
-   Instead of checking for error returns in main, we inject `print_error(NULL)` directly into the `calloc`/`malloc` of `extract_message` and `str_join`.

### Formatting Gotchas
-   `server: client %d just arrived\n` — note the space after `:` and before `client`
-   `server: client %d just left\n` — same pattern
-   `client %d: ` — note the space after `:`

---

## 9. How to Train for the Exam (Muscle Memory)

The 42 exam doesn't just test if you "understand" the concepts; it tests your **Muscle Memory** under stress. Follow this training method to guarantee you pass:

### The Golden Rule: Erase everything if you make a mistake
Never "fix" your code by looking at the reference. If you get stuck or the code doesn't compile, you can look at the guide to see where you went wrong. But your punishment must be to **erase the entire function (or file) and type it again from scratch**. This forces your brain to build the correct neural pathway.

### Modular Training
Follow the same Phase strategy from the next section. Train in isolated blocks, moving forward only when the current block flows smoothly:
1. **Block A (Basic Setup):** Memorize the 3 extra headers, the global variables, and the `t_client` struct.
2. **Block B (Utilities):** Practice writing `print_error`, `sendAll`, and `processMessages`. They are short and easy to memorize.
3. **Block C (Setup and Heart):** Train `init_server` and the beast: `run_server` (the `while(1)`, `select`, and `FD_ISSET` inner `for` loop).
4. **Block D (The Branches):** Finally, train `accept_client` and `handle_client`.

### The "42 Mock Exam"
When you feel you've mastered the blocks, open a raw editor (no plugins), copy only the original `main.c`, set a 45-minute timer, and try to write it from scratch. Test it by connecting two terminals with `nc 127.0.0.1 8080`. If you follow this method, you'll finish the real exam in under 15 minutes!

---

## 10. Step-by-Step Writing Strategy (Exam Day)

Follow this order to write the code from scratch in the exam:

### Phase 1: Copy the provided code (2 min)
1. Copy `extract_message` and `str_join` from the provided `main.c` — and modify the memory allocation failures to call `print_error(NULL)` directly.
2. Add the extra headers (and why they are needed):
    - `<sys/select.h>`: To use `select` and the macros `fd_set`, `FD_ZERO`, etc.
    - `<stdlib.h>`: For memory management (`malloc`, `free`) and `exit()`.
    - `<stdio.h>`: Exclusively to use the `sprintf` function.

### Phase 2: Globals + struct (1 min)
```c
typedef struct s_client { int id; char *msg; } t_client;
t_client clients[FD_SETSIZE];
char buffer_msg[128];
int serverfd = -1, maxfds = 0, current_id = -1;
fd_set master, readfds;
```

### Phase 3: Utilities and Errors (2 min)
*(We write them first so we can code the rest of the file top-to-bottom without needing prototypes)*
1. Create `print_error()` — writes to stderr and calls exit(1).
2. Create `sendAll()` — loop 0..maxfds, checks if in master, ignores serverfd and sender, and calls `send()`.
3. Create `processMessages()` — sprintf prefix "client X:", loop with `extract_message`, send using `sendAll`, and `free(msg)`.

### Phase 4: The Setup (init_server) (2 min)
1. Create `init_server(int port)` reusing the boilerplate from the original `main.c` (socket, bzero, bind, listen). Inside it, initialize `serverfd`, update `maxfds`, and clear the `master` set.

### Phase 5: The Network Branches (4 min)
*(We write them before run_server so it can call them directly)*
1. Create `accept_client()`: Runs `accept`, adds to the `master` set, registers the new client's ID and null string, and notifies the room of the arrival via `sendAll()`.
2. Create `handle_client(fd)`: Uses `recv`. If bytes <= 0, the client disconnected (clears from master, closes socket, notifies the room). If text arrives, concatenates with `str_join` and passes it to `processMessages()`.

### Phase 6: The Server Heart (run_server) (3 min)
1. Create `run_server()` containing the `while(1)` loop.
2. Mirror `master` to `readfds` and call `select`.
3. Create the inner `for` loop (0 up to `maxfds`) listening to `FD_ISSET`.
4. If the `serverfd` light turns on, call `accept_client()`. Otherwise, call `handle_client(fd)`.

### Phase 7: Main Function (1 min)
1. Write a clean and short `main` at the end of the file: validates `argc != 2`, calls `init_server()` passing the port, and then calls `run_server()`.

### Phase 8: Test (5 min)
```bash
gcc -Wall -Wextra -Werror en/array/mini_serv.c -o mini_serv
./mini_serv 8080
# In another terminal:
nc 127.0.0.1 8080
# In yet another terminal:
nc 127.0.0.1 8080
```

---

## 11. Cheat Sheet Logic Flow

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
            update maxfds
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

## 11. Knowledge Check (Quiz)

Test your knowledge with short questions. If you can answer these, you're ready to defend your code during the evaluation!

<details>
<summary><b>1. Why is it necessary to use <code>extract_message</code> when receiving data via TCP?</b></summary>
TCP is a stream protocol. Messages can arrive fragmented or grouped together. The <code>extract_message</code> function ensures the server processes one complete line at a time.
</details>

<details>
<summary><b>2. What is the critical behavior of the <code>str_join</code> function regarding memory management?</b></summary>
It automatically calls <code>free()</code> on the pointer passed as its first argument after concatenation, requiring care to avoid double-frees or leaks.
</details>

<details>
<summary><b>3. What should be done if <code>recv()</code> returns 0?</b></summary>
A return of 0 indicates the client closed the connection (EOF). The server must notify others ("just left"), remove the client from the <code>master</code> set, close its socket (fd), and <code>free()</code> its buffer.
</details>

<details>
<summary><b>4. Why can't you pass the <code>master</code> set directly to <code>select()</code>?</b></summary>
Because <code>select()</code> is destructive. It will modify the set to show only the ready descriptors. If you pass <code>master</code> and it gets modified, you will lose the record of all other connected clients for the next iterations.
</details>

<details>
<summary><b>5. Why do we omit <code>writefds</code> in the exam?</b></summary>
<div align="left">

In real networking, we use <code>writefds</code> to ensure the OS has space in the client's TCP buffer before calling <code>send()</code>, preventing the server from blocking (freezing) waiting for slow clients. Since the exam tests the code locally and Moulinette's "clients" read the small text blocks instantly, there is no risk of the buffer filling up. Omitting <code>writefds</code> and passing `NULL` greatly simplifies the code with no risks.

</div>
</details>

---

## 12. Glossary of Key Terms

| Term | Definition |
|---|---|
| **I/O Multiplexing** | A technique allowing a single process to monitor multiple I/O channels at the same time (via `select()`). |
| **File Descriptor (fd)** | A non-negative integer index maintained by the kernel that points to an open file or socket by a process. |
| **Non-blocking** | A mode of operation where a system call returns immediately if it cannot complete its task, rather than pausing execution. |
| **Broadcasting** | Sending a message received from one participant to all other participants in the network simultaneously. |
| **Socket** | An endpoint of a two-way communication link between two programs running on the network. |
| **EOF (End Of File)** | A condition reached when there is no more data to read from a stream, frequently indicating a disconnect on sockets. |
