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
	gcc -g -Wall -o t1 main.c crud.c binarioNaTela.c -D DEBUG

zip:
	zip 9364890 main.c crud.* binarioNaTela.* makefile
