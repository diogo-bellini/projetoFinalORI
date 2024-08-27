CC = gcc
CFLAGS = -Wall -g
EXEC = program
SRC = sources/main.c sources/ArvoreAVL.c sources/ArvoreAVL_set.c sources/Set.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC)