CC = gcc
TARGET = main
DEPENDENCY = main.c error.c

$(TARGET):  $(DEPENDENCY)
	$(CC) -o $@ $^

clean :
	rm -rf *.o $(TARGET)
