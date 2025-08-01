CC = gcc
CFLAGS = -Wall -Wextra -pthread -I./functions/general -I./functions/tasks -I./functions/users
DEBUGFLAGS = -g

OBJ = client.o \
      functions/general/sendReq.o \
      functions/general/ui.o \
      functions/users/userData.o \
      functions/users/userUI.o \
      functions/tasks/taskData.o \
      functions/tasks/taskRbTree.o \
      functions/tasks/taskUI.o

# regra pra gerar .o de debug com nomes diferentes
%_debug.o: %.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -c $< -o $@

all: client

client: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# alvo de debug
debug: clean_debug
	$(MAKE) BIN=client_debug CFLAGS="$(CFLAGS) $(DEBUGFLAGS)" OBJ="$(OBJ:.o=_debug.o)" build_debug

build_debug: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

clean:
	rm -f *.o functions/general/*.o functions/tasks/*.o functions/users/*.o client

clean_debug:
	rm -f *_debug.o functions/general/*_debug.o functions/tasks/*_debug.o functions/users/*_debug.o client_debug
