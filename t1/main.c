/***************************************
 * Autores :    10786121 Luiza Rubio 
 *              9364890 Ricardo Araujo
 * ****************************************/
#include"crud.h"
#include"binarioNaTela.h"

int csv2bin(char *csv, char *bin){
        	// opening files pointers
	FILE *input = fopen(csv,"r"), *output = fopen(bin, "w+b");
    
    	    	// checking for null file pointers
    	if ( input == NULL || output == NULL ){
    	    printf("Falha no processamento do arquivo.");
    	    return 0; 
    	}
    	    
    	char aux = 0;
    	        // starting initial registers to zero
    	HEAD head = {0};
    	REG reg = {0};
    	
    	    	// writting header into binary file
    	head.status = '0';
    	bwrite_head(output, &head);
    	    
    	    /* csv first row has only column names
    	     * ... so... we're going to jump it. */
    	while(aux = fgetc(input)){
    	    if(aux == '\n' || aux == EOF)
    	        break;
    	}
    	    
    	    	// writting new registers until EOF
    	while(!mfeof(input)){
    	        // read input line
    	    fread_reg(input,&reg);
#ifdef DEBUG
    	    print_reg(&reg);
#endif
    	        // updating header struct values
    	    head.numeroRegistrosInseridos++;
    	    head.RRNproxRegistro++; 
    	    
    	        // writting to binary file
    	    bwrite_reg(output, &reg);
    	}
    	    // updating status byte
    	head.status = '1'; 
    	    // writting header again, with recent values and 
    	bwrite_head(output, &head);
    	    // closing file pointers
	fclose(input);
	fclose(output);
	return 0;
}

int bin2screen(char *bin){
        	// opening files pointers
	FILE *fp=NULL;
    	int i = 0;
    	    fp = fopen(bin, "rb");
    	    // checking for null file pointers
    	if(fp == NULL){
    	    printf("Falha no processamento do arquivo.");
    	    return 0; 
    	}

    	        // starting initial registers to zero
    	HEAD head = {0};
    	REG reg = {0};
    	    // reads header
    	bread_head(fp, &head);
    	    
    	    // exit if inconsistency
    	if( head.status != '1' ){
    	    printf("Falha no processamento do arquivo.");
    	    fclose(fp);
    	    return 0; 
    	}
    	
    	    // checks if there are no regs
    	if(head.numeroRegistrosInseridos == 0)
    	    printf("Registro inexistente");
   
    	  // salva a posição antes do loop
    	int position= ftell(fp);
    	int aux=0;

    	    // goes through every reg printing them
    	while(i++ < head.numeroRegistrosInseridos){
    	    // read registry on binary
    	        bread_reg(fp, &reg);
    	    // print its values
    	        print_reg(&reg);
    	    // atualiza ponteiro pro prox registro
    	        aux=position+SIZEOF_REG;
    	        fseek(fp,aux,SEEK_SET);
    	        position=aux;

    	}   
        
	fclose(fp);
	return 0;
}

int rrn2screen(char *bin, int rrn){
    	    // opening files pointers
	FILE *b = fopen(bin, "rb");
    
        // checking for null file pointers
    	if ( b == NULL ){
    	    printf("Falha no processamento do arquivo.");
    	    return 0; 
    	}
    	    
    	        // starting initial registers to zero
    	HEAD head = {0};
    	REG reg = {0};
    	
    	    // reads header
    	bread_head(b, &head);

    	    // exit if inconsistency
    	if( head.status != '1' ){
    	    printf("Falha no processamento do arquivo.");
    	    fclose(b);
    	    return 1; 
    	}
    	
    	    // checks if there are no regs
    	if(head.numeroRegistrosInseridos == 0){
    	    printf("Registro inexistente");
    	    return 1;
    	}   

    	    // moving accordingly documentation suggests
    	fseek(b, rrn*SIZEOF_REG, SEEK_SET);
    	    
    	    // reading on position in binary file
    	bread_reg(b, &reg);
    	print_reg(&reg);

    	fclose(b);
    	return 0;
}

int search(char *bin, REG *reg, char mask[8]){
	        // opening files pointers
	FILE *b = fopen(bin, "rb");
    
    	    // checking for null file pointers
    	if ( b == NULL ){
    	    printf("Falha no processamento do arquivo.");
    	    return 0; 
    	}
    	    
    	        // starting initial registers to zero
    	HEAD head = {0};
    	REG reg = {0};
    	
    	    // reads header
    	bread_head(b, &head);

        // exit if inconsistency
    	if( head.status != '1' ){
        	printf("Falha no processamento do arquivo.");
        	fclose(b);
        return 1; 
    	}
    
//        // checks if there are no regs
//    if(head.numeroRegistrosInseridos == 0){
//        printf("Registro inexistente");
//        return 1;
//    }   

        
        	// reading on position in binary file
    	//bread_reg(b, &reg);
    
    	//print_reg(&reg);

   	fclose(b);
	return 0;
}

int main(void){
        // setting getline variables to read input
	size_t size = GETLINE_RECOMMENDED_SIZE;
	char *args, *in, *out, *aux1, *aux2, tmp[MAX_SIZE];
    args = in = out = aux1 = aux2 = NULL;
	int op, m, m2;
    op = m = m2 = 0;
        // reading input
	getline(&args, &size, stdin);
	space_converter(args);
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
    }else if( op == 3 ){
	    // search on binary file for regs given some restrictions 
		// auxiliar registry struct to save input
	REG reg = {0};
		// mask to specify what to change
	char mask[8] = {0};
        in = strtok(NULL, " \n");
            // reading number of arguments
        op = atoi(strtok(NULL, " \n"));
        while(op--){
		// receiving NomeDoCampon
            aux1 = strtok(NULL, " \n");
	   	// ... and its value
            aux2 = strtok(NULL, " \n");
    
		// treating values and setting mask
	    if (strcmp("cidadeMae", aux1) == 0){
		    mask[0]++;
		    strcpy(tmp, aux2);
		    quotes_clean(tmp);

		    reg.sizeCidadeMae = strlen(tmp);
		    if(strlen(tmp) > 0 )	strcpy(reg.CidadeMae, tmp);
	    }else if (strcmp("cidadeBebe", aux1) == 0){
		    mask[1]++;
		    strcpy(tmp, aux2);
		    quotes_clean(tmp);

		    reg.sizeCidadeBebe = strlen(tmp);
		    if(strlen(tmp) > 0 )	strcpy(reg.CidadeBebe, tmp);
	    }else if (strcmp("idNascimento", aux1) == 0){
		    mask[2]++;
		    strcpy(tmp, aux2);
		    quotes_clean(tmp);
		    
		    reg.idNascimento = atoi(tmp);
	    }else if (strcmp("idadeMae", aux1) == 0){
		    mask[3]++;
		    strcpy(tmp, aux2);
		    quotes_clean(tmp);

		    reg.idadeMae = atoi(tmp);
	    }else if (strcmp("dataNascimento", aux1) == 0){ 
		    mask[4]++;
		    strcpy(tmp, aux2);
		    quotes_clean(tmp);

		    strcpy(reg.dataNascimento, tmp);
	    }else if (strcmp("sexoBebe", aux1) == 0){
		    mask[5]++;
		    strcpy(tmp, aux2);
		    quotes_clean(tmp);

		    reg.sexoBebe = tmp[0];
	    }else if (strcmp("estadoMae", aux1) == 0){
		    mask[6]++;
		    strcpy(tmp, aux2);
		    quotes_clean(tmp);

		    strcpy(reg.estadoMae, tmp);
	    }else if (strcmp("estadoBebe", aux1) == 0){
		    mask[7]++;
		    strcpy(tmp, aux2);
		    quotes_clean(tmp);

		    strcpy(reg.estadoBebe, tmp);
	    }else{
	    	// error case, invalid nomeDoCampo
	    }

        }
        // call a search function by provided partial register, returning
        // reg(s) or rrn and print it
    }else if( op == 4 ){
        in = strtok(NULL, " \n");
        op = atoi(strtok(NULL, " \n"));
        // call a getbyrrn function, return reg and print it.
        rrn2screen(in, op);
    }else if( op == 5 ){
        in = strtok(NULL, " \n");
        op = atoi(strtok(NULL, " \n"));
        while(op--){
                // reads a input line in unuset char *out
                //          check if size reusage could lead to bugs
            getline(&out, &size, stdin); 
            m = atoi(strtok(out, " \n"));
            while(m--){
                aux1 = strtok(NULL, " \n");
                aux2 = strtok(NULL, " \n");
                // append to somewhere ?
            }
            free(out);
            // call a search function by provided partial register
            // remove register
        }
        binarioNaTela(in);
        // call to func 5
    }else if( op == 6 ){
        in = strtok(NULL, " \n");
        op = atoi(strtok(NULL, " \n"));
        while(op--){
            // getline(&out, &size, stdin); ?
            // strip data from line
            // write on reg
            //          fwrite_reg(reg);
            // reg to file
        }
        // call to func 6
    }else if( op == 7 ){
        in = strtok(NULL, " \n");
        op = atoi(strtok(NULL, " \n"));
        while(op--){
            // getline(&out, &size, stdin);
            //
            // free(out);
                // call to func 7, updating a reg on .bin
        }
    }
        // getline memory free
	free(args);	

	return 0;
}
