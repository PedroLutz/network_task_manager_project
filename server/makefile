CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./functions/general -I./functions/tasks -I./functions/users
OBJ = server.o \
      functions/general/modeParser.o \
      functions/general/sync.o \
      functions/tasks/taskDb.o \
      functions/tasks/taskReqHandler.o \
      functions/users/userDb.o \
      functions/users/userReqHandler.o \
      functions/users/userHash.o \
      functions/users/userCache.o \

all: server

server: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o functions/general/*.o functions/tasks/*.o functions/users/*.o server
