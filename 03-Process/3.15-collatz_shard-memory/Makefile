CC = gcc
TARGET = main
DEPENDENCY = collatz.c error.c main.c
FLAG = -lrt

$(TARGET):  $(DEPENDENCY)
	$(CC) -o $@ $^ $(FLAG)

clean :
	rm -rf *.o $(TARGET)
