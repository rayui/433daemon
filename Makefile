PROJ=433daemon
CC=cc
SRC=main.c parser.c gpioline.c output.c ook.c
LIBS=-lgpiod
CFLAGS=-D CONSUMER=\"$(PROJ)\" -Iinclude/ -static

all:
	$(CC) $(SRC) $(LIBS) $(CFLAGS) -o $(PROJ)

clean:
	rm $(PROJ)
