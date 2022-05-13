obj = main.o ./queue/queue.o ./queue/read.o

minicron: $(obj)
	gcc $(obj) -o ./bin/minicron

$(obj): ./queue/queue.h

.PHONY : clean
clean:
	rm ./bin/minicron *.o