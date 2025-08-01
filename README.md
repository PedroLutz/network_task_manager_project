# Task List Management System with Client-Server in C

This is a project developed in C that implements a Task List Management system, utilizing TCP sockets for client-server communication. The server supports multiple concurrent conections, task ordering, data manipulation and file persistence.

# Functionalities

### Client
- Interface via terminal (Simple and organized UI)
- Local task storage using Red-Black Trees
- Multiple ordering modes (ID, description, priority)
- Server communication via TCP

### Server
- Local user storage using a Hash Table
- File manipulation for data persistence
- Multiple concurrent connections using `pthreads`
- Access controlled with `mutex` to avoid race conditions
- Reliable communication using TCP sockets

## Tecnologies and applied concepts
- C Programming Language
- Sockets TCP/IP
- Threads with `pthreads`
- Syncing with `mutex`
- Hash Table (server)
- Red-Black Trees (client)
- Dynamic memory allocation and liberation
- C file manipulation (`fopen`, `fread`, `fwrite`, etc.)

## How to run
***This system only runs on Linux.***
### Server
```bash
cd server
make
./server
```
### Client
```bash
cd client
make
./client
```
Obs: Server and client must be connected to the same network or with IP properly configured.

## Tests and Validation
- Tested with multiple clients simultaneously
- Checked with `valgrind` to ensure lack of memory leakages
- Error cases treated with clear terminal messages

## Learnings
This project helped me apply fundamental and advanced concepts in C, with a focus on:
- Efficient data structures
- Concurrency and parallelism
- Network communication with sockets
- Manual memory management
- Quick responses from server to client

## Possible future improvements
- Cryptographed user passwords
- Graphic interface or web integration
- Server activity logs
- Docker deployment

# Author
- **Pedro Guilherme Rosa Lutz**
- **Email: pedrolutz@protonmail.com**
- **LinkedIn: https://www.linkedin.com/in/pedro-lutz-4001ba221/**
- **Github: PedroLutz**

## Images
<img width="372" height="163" alt="image" src="https://github.com/user-attachments/assets/659922c6-4f35-432a-beed-7809f155204e" />
<img width="342" height="189" alt="image" src="https://github.com/user-attachments/assets/648555f6-5052-4ffe-99ab-082825bbbead" />
<img width="345" height="170" alt="image" src="https://github.com/user-attachments/assets/63a8e728-559a-4b38-a02c-96747c2dcb57" />

