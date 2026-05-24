CC = gcc

CFLAGS = -Wall -pthread

SRC = main.c server.c worker.c queue.c logger.c

OUT = chat_server

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)

clean:
	rm -f $(OUT)
