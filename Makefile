CC = gcc
CFLAGS = -Wall -g
EXEC = program
SRC = sources/main.c sources/ArvoreAVL.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC)