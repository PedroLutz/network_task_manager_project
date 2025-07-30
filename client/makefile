CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./functions/general -I./functions/tasks -I./functions/users
OBJ = client.o \
      functions/general/sendReq.o \
      functions/general/ui.o \
	  functions/users/userData.o \
	  functions/users/userUI.o \
	  functions/tasks/taskData.o \
	  functions/tasks/taskRbTree.o \

all: client

client: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o functions/general/*.o functions/tasks/*.o functions/users/*.o client
