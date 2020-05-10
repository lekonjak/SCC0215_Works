all:	compile

clean:
	rm *.o

run:
	./t1

compile:
	gcc -c crud.c
	gcc -c binarioNaTela.c
	gcc -o t1 main.c crud.o binarioNaTela.o
debug:
	gcc -g -Wall -c binarioNaTela.c -D DEBUG
	gcc -g -Wall -c crud.c -D DEBUG
	gcc -g -Wall -o t1 main.c crud.o binarioNaTela.o -D DEBUG
