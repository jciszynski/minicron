OBJ = main.o read.o queue.o utest.o

all: hello

hello: $(OBJ)
	gcc $(OBJ) -o minicron

$(OBJ): read.h queue.h utest.h

.PHONY: clean
clean:
	rm -f *.o
