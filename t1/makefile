all:	compile

clean:
	rm *.o

run:
	./programaTrab

compile:
	gcc -c crud.c
	gcc -c binarioNaTela.c
	gcc -o programaTrab main.c crud.o binarioNaTela.o
debug:
	gcc -g -Wall -o programaTrab main.c crud.c binarioNaTela.c -D DEBUG

zip:
	zip 9364890 main.c crud.* binarioNaTela.* makefile
