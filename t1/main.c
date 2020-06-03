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
		// checking for removed reg
	if (reg.sizeCidadeMae >= 0) print_reg(&reg);

    	fclose(b);
    	return 0;
}

int search(char *bin, REG *reg, char mask[8]){
	        // opening files pointers
	FILE *b = fopen(bin, "rb");
    	int removed = -1;
    	    // checking for null file pointers
    	if ( b == NULL ){
    	    	printf("Falha no processamento do arquivo.");
    	    	return 0; 
    	}
    	    
    	        // starting initial registers to zero
    	HEAD head = {0};
    	REG auxReg = {0};
    	
    	   	// reads header
    	bread_head(b, &head);
        	// exit if inconsistency
    	if( head.status != '1' ){
        	printf("Falha no processamento do arquivo.");
        	fclose(b);
        return 1; 
    	}
    
        	// reading on position in binary file
    	while(!feof(b)){
		bread_reg(b, &auxReg);
			// checking for removed reg
		if (auxReg.sizeCidadeMae == removed) continue;

		if (mask[0]){
			// comparing cidadeMae 
			if ( reg->sizeCidadeMae != auxReg.sizeCidadeMae  || \
					strcmp(reg->cidadeMae, auxReg.cidadeMae) != 0) continue;
		}
		if (mask[1]){
			// comparing cidadeBebe 
			if ( reg->sizeCidadeBebe != auxReg.sizeCidadeBebe  || \
					strcmp(reg->cidadeBebe, auxReg.cidadeBebe) != 0) continue;
		}
		if (mask[2]){
			// comparing idNascimento
			if ( reg->idNascimento != auxReg.idNascimento) continue;
		}
		if (mask[3]){
			// comparing idadeMae
			if ( reg->idadeMae != auxReg.idadeMae) continue;	
		}
		if (mask[4]){
			// comparing dataNascimento
			if ( strcmp(reg->dataNascimento, auxReg.dataNascimento) != 0) continue;
		}
		if (mask[5]){
			// comparing sexoBebe
			if ( reg->sexoBebe != auxReg.sexoBebe ) continue;
		}
		if (mask[6]){
			// comparing estadoMae
			if ( strcmp(reg->estadoMae, auxReg.estadoMae) != 0) continue;
		}
		if (mask[7]){
			// comparing estadoBebe
			if ( strcmp(reg->estadoBebe, auxReg.estadoBebe) != 0) continue;
		}
	
		// theoretically, if it gets here without jumping to loop next iteration, it matches search criteria
		print_reg(&auxReg);	
	}
    

   	fclose(b);
	return 0;
}

int search2remove(char *bin, REG *reg, char mask[8]){
	        // opening files pointers
	FILE *b = fopen(bin, "rb+");
       	long fp;
	int removed = -1;
    
    	    // checking for null file pointers
    	if ( b == NULL ){
    	    	printf("Falha no processamento do arquivo.");
    	    	return 0; 
    	}
    	    
    	        // starting initial registers to zero
    	HEAD head = {0};
    	REG auxReg = {0};
    	
    	   	// reads header
    	bread_head(b, &head);
// TODO change status byte before and after this function

        	// exit if inconsistency
    	if( head.status != '1' ){
        	printf("Falha no processamento do arquivo.");
        	fclose(b);
        return 1; 
    	}
    
        	// reading on position in binary file
    	while(!feof(b)){
			// saving register start, to write in this position in case of removal
		fp = ftell(b);
			// reading the binary file
		bread_reg(b, &auxReg);
			
			// checking for removed reg
		if (auxReg.sizeCidadeMae < 0) continue;
		
		if (mask[0]){
			// comparing cidadeMae 
			if ( reg->sizeCidadeMae != auxReg.sizeCidadeMae  || \
					strcmp(reg->cidadeMae, auxReg.cidadeMae) != 0) continue;
		}
		if (mask[1]){
			// comparing cidadeBebe 
			if ( reg->sizeCidadeBebe != auxReg.sizeCidadeBebe  || \
					strcmp(reg->cidadeBebe, auxReg.cidadeBebe) != 0) continue;
		}
		if (mask[2]){
			// comparing idNascimento
			if ( reg->idNascimento != auxReg.idNascimento) continue;
		}
		if (mask[3]){
			// comparing idadeMae
			if ( reg->idadeMae != auxReg.idadeMae) continue;	
		}
		if (mask[4]){
			// comparing dataNascimento
			if ( strcmp(reg->dataNascimento, auxReg.dataNascimento) != 0) continue;
		}
		if (mask[5]){
			// comparing sexoBebe
			if ( reg->sexoBebe != auxReg.sexoBebe ) continue;
		}
		if (mask[6]){
			// comparing estadoMae
			if ( strcmp(reg->estadoMae, auxReg.estadoMae) != 0) continue;
		}
		if (mask[7]){
			// comparing estadoBebe
			if ( strcmp(reg->estadoBebe, auxReg.estadoBebe) != 0) continue;
		}
	
		// theoretically, if it gets here without jumping to loop next iteration, it matches search criteria
			// updating removed register counter
		head.numeroRegistrosRemovidos++;
			// moving to register start
		fseek(b,fp, SEEK_SET);
			// erasing register
		fwrite(&removed, sizeof(int), 1,b);
			// moving to next register
		fseek(b,SIZEOF_REG - sizeof(int), SEEK_CUR);

	}
    
   	fclose(b);
	return 0;
}
int main(void){
        	// setting getline variables to read input
	size_t size = GETLINE_RECOMMENDED_SIZE;
	char *args, *in, *out, *aux1, *aux2, tmp[MAX_SIZE], mask[8];
		// auxiliar registry struct to save input
	REG reg = {0};
			// mask to specify what to change
    	args = in = out = aux1 = aux2 = NULL;
	int op, m, m2;
    	op = m = m2 = 0;
        // reading input
	getline(&args, &size, stdin);
	space_converter(args);
	quotes_clean(args);
#ifdef DEBUG
			
		    printf("%s", args);
#endif
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
		memset(mask, 0, sizeof(char)*8);
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
			    space_return(aux2);
			    reg.sizeCidadeMae = strlen(aux2);
			    if(strlen(aux2) > 0 )	strcpy(reg.cidadeMae, aux2);
		    }else if (strcmp("cidadeBebe", aux1) == 0){
			    mask[1]++;
			    space_return(aux2);
			    reg.sizeCidadeBebe = strlen(aux2);
			    if(strlen(aux2) > 0 )	strcpy(reg.cidadeBebe, aux2);
		    }else if (strcmp("idNascimento", aux1) == 0){
			    mask[2]++;
			    reg.idNascimento = atoi(aux2);
		    }else if (strcmp("idadeMae", aux1) == 0){
			    mask[3]++;
			    reg.idadeMae = atoi(aux2);
		    }else if (strcmp("dataNascimento", aux1) == 0){ 
			    mask[4]++;
			    strcpy(reg.dataNascimento, aux2);
		    }else if (strcmp("sexoBebe", aux1) == 0){
			    mask[5]++;
			    reg.sexoBebe = aux2[0];
		    }else if (strcmp("estadoMae", aux1) == 0){
			    mask[6]++;
			    strcpy(reg.estadoMae, aux2);
		    }else if (strcmp("estadoBebe", aux1) == 0){
			    mask[7]++;
			    strcpy(reg.estadoBebe, aux2);
		    }else{
		    	// error case, invalid nomeDoCampo
		    }
        	}
		search(in, &reg, mask);
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
				// cleaning mask array	
			memset(mask, 0, sizeof(char)*8);
      		         	// reads a input line in unuset char *out
            		getline(&out, &size, stdin); 
				// dealing with quotes and spaces to strtok usage
			space_converter(out);
			quotes_clean(out);
				// reading number of arguments
            		m = atoi(strtok(out, " \n"));
            		while(m--){
					// receiving NomeDoCampon
        	    		aux1 = strtok(NULL, " \n");
		    		   	// ... and its value
        	    		aux2 = strtok(NULL, " \n");
		    		   	// treating values and setting mask
		    		if (strcmp("cidadeMae", aux1) == 0){
		    		        mask[0]++;
		    		        space_return(aux2);
		    		        reg.sizeCidadeMae = strlen(aux2);
		    		        if(strlen(aux2) > 0 )	strcpy(reg.cidadeMae, aux2);
		    		}else if (strcmp("cidadeBebe", aux1) == 0){
		    		        mask[1]++;
		    		        space_return(aux2);
		    		        reg.sizeCidadeBebe = strlen(aux2);
		    		        if(strlen(aux2) > 0 )	strcpy(reg.cidadeBebe, aux2);
		    		}else if (strcmp("idNascimento", aux1) == 0){
		    		        mask[2]++;
		    		        reg.idNascimento = atoi(aux2);
		    		}else if (strcmp("idadeMae", aux1) == 0){
		    		        mask[3]++;
		    		        reg.idadeMae = atoi(aux2);
		    		}else if (strcmp("dataNascimento", aux1) == 0){ 
		    		        mask[4]++;
		    		        strcpy(reg.dataNascimento, aux2);
		    		}else if (strcmp("sexoBebe", aux1) == 0){
		    		        mask[5]++;
		    		        reg.sexoBebe = aux2[0];
		    		}else if (strcmp("estadoMae", aux1) == 0){
		    		        mask[6]++;
		    		        strcpy(reg.estadoMae, aux2);
		    		}else if (strcmp("estadoBebe", aux1) == 0){
		    		        mask[7]++;
		    		        strcpy(reg.estadoBebe, aux2);
		    		}else{
		    			// error case, invalid nomeDoCampo
		    		}
            		}
            		free(out);
            		// call a search function by provided partial register
            		// remove register
			search2remove(in, &reg, mask);
        	}
        	binarioNaTela(in);
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
