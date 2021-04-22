.SUFFIXES : .c .o

TARGET = 4.16
DEPENDENCY = 4.16.c
CFLAGS = -pthread

$(TARGET):  $(DEPENDENCY)
	gcc $^ $(CFLAGS) -o $(TARGET)

clean :
	rm -rf a.out *.o $(TARGET)
