CC = gcc
TARGET = main
DEPENDENCY = collatz.c main.c error.c

$(TARGET):  $(DEPENDENCY)
	$(CC) -o $@ $^

clean :
	rm -rf *.o $(TARGET)
