all:	compile

run:
	./t1

compile:
	gcc -c crud.c
	gcc -o t1 main.c crud.o
debug:
	gcc -g -Wall -c crud.c -D DEBUG
	gcc -g -Wall -o t1 main.c crud.o -D DEBUG
