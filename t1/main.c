#include"crud.h"
#include"binarioNaTela.h"

int csv2bin(char *csv, char *bin){
        // opening files pointers
	FILE *input = fopen(csv,"r"), *output = fopen(bin, "w+b");
	    // starting initial registers to zero
    HEAD head = {0};
    REG reg = {0};
        // writting header into binary file
    bwrite_head(output, &head);
        // csv first row has only column names
        // ... so... we're going to jump it.
    fscanf(input, "%*[^\n]");
    fseek(input, 1, SEEK_CUR);
        // writting new registers until EOF
    while(!mfeof(input)){
            // read input line
        fread_reg(input,&reg);
#ifdef DEBUG
        print_reg(&reg);
#endif
            // counting regs 
        head.numeroRegistrosInseridos++;
            // writting to binary file
        bwrite_reg(output, &reg);
    }
        // updating status byte
    head.status = 1; 
        // writting header again, with recent values and 
    bwrite_head(output, &head);
        // closing file pointers
	fclose(input);
	fclose(output);
	return 0;
}

int bin2screen(char *bin){

	FILE *fp=NULL;
    int i = 0;
	fp = fopen(bin, "rb");
        //in case it didnt open//
    if(fp == NULL){
        printf("Falha no processamento do arquivo");
        return 0; 
    }

    HEAD head = {0};
    REG reg = {0};
        //reads header//
    bread_head(fp, &head);

    //checks if there are no regs//
    if(head.numeroRegistrosInseridos == 0)
        printf("Registro inexistente");
   
        //goes through every reg printing them
    while(i++ < head.numeroRegistrosInseridos){
        // read registry on binary
            bread_reg(fp, &reg);
        // print its values
            print_reg(&reg);
    }   

	fclose(fp);
	return 0;
}

int main(void){
        // setting getline variables to read input
	size_t size = GETLINE_RECOMMENDED_SIZE;
	char *args = NULL, *in = NULL, *out = NULL;
	int op = 0;
        // reading input
	getline(&args, &size, stdin);
        // casting operation to integer
	    op = atoi(strtok(args," \n"));
        // Operation selection
	if( op == 1 ){
            // reads csv file and output to binary file
        in = strtok(NULL, " \n");
        out = strtok(NULL, " \n");
		csv2bin(in, out);
        binarioNaTela(out);
	}else if( op == 2 ){
        // reads binary file and outputs to stdout
		bin2screen(strtok(NULL, " \n"));	
	}
        // getline memory free
	free(args);	

	return 0;
}
