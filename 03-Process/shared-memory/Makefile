EXE = producer consumer
FLAG = -lrt
CC = gcc

all: consumer producer

consumer: consumer.c
	$(CC) -o $@ consumer.c $(FLAG)

producer: producer.c
	gcc -o $@ producer.c $(FLAG)

clean:
	rm -rf $(EXE)
