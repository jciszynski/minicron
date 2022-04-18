OBJ = main.o queue/read.o queue/queue.o utest.o

all: hello

hello: $(OBJ)
	gcc $(OBJ) -o minicron

$(OBJ): queue/read.h queue/queue.h utest.h

.PHONY: clean
clean:
	rm -f *.o
