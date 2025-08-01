# Task List Management System with Client-Server in C

This project is a task list management system developed in C, using TCP sockets for client-server communication. The server supports multiple concurrent connections, task organization, data manipulation, and file persistence.

# Functionalities

### Client
- Interface via terminal (Simple and organized UI)
- Local task storage using Red-Black Trees
- Multiple sorting modes (ID, description, priority)
- Communication with the server via TCP

### Server
- Local user storage using a hash table
- File manipulation for data persistence
- Multiple concurrent connections using `pthreads`
- Thread-safe access using `mutex` to avoid race conditions
- Reliable communication through TCP sockets

## Tecnologies and applied concepts
- C Programming Language
- Sockets TCP/IP
- Multithreading with `pthreads`
- Synchronization with `mutex`
- Hash Table (server-side)
- Red-Black Trees (client-side)
- Dynamic memory allocation and deallocation
- File I/O in C (`fopen`, `fread`, `fwrite`, etc.)

## How to run
***This system runs on Linux only.***
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
- Checked with `valgrind` to ensure no memory leaks
- Error cases handled with clear terminal messages

## Learnings
This project helped me apply both fundamental and advanced concepts in C, with a focus on:
- Efficient data structures
- Concurrency and parallelism
- Network communication using sockets
- Manual memory management
- Fast server-client response handling

## Possible future improvements
- Encrypted user passwords
- Graphic interface or web integration
- Server activity logging
- Docker-based deployment

# Author
- **Pedro Guilherme Rosa Lutz**
- **Email: pedrolutz@protonmail.com**
- **LinkedIn: https://www.linkedin.com/in/pedro-lutz-4001ba221/**
- **Github: PedroLutz**

## Images
<img width="372" height="163" alt="image" src="https://github.com/user-attachments/assets/659922c6-4f35-432a-beed-7809f155204e" />
<img width="342" height="189" alt="image" src="https://github.com/user-attachments/assets/648555f6-5052-4ffe-99ab-082825bbbead" />
<img width="345" height="170" alt="image" src="https://github.com/user-attachments/assets/63a8e728-559a-4b38-a02c-96747c2dcb57" />

