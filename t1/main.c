#include"crud.h"

int csv2bin(char *csv, char *bin){
	FILE *input = fopen(csv,"r"), *output = fopen(bin, "w+b");
	
	HEAD *head = malloc (gt;


	fclose(input);
	fclose(output);
	return 0;
}

int bin2screen(char *bin){
	//to do
	FILE *fp = fopen(bin, "rb");

	fclose(fp);
	return 0;
}

int main(void){
	size_t size;
	char *args;
	int op;
	getline(&args, &size, stdin);
	printf("%s", args);
	op = atoi(strtok(args," \n"));
	if( op == 1 ){
		//printf("'%s' '%s'\n", strtok(NULL," \n"), strtok(NULL," \n"));
		csv2bin(strtok(NULL," \n"),strtok(NULL," \n"));
	}else if( op == 2 ){
		bin2screen(strtok(NULL, " \n"));	
	}
	free(args);	

	return 0;
}
