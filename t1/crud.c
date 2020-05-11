#include"crud.h"

    // writting registry/header to binary
int bwrite_reg(FILE *fp, REG *reg){
    fseek(fp, 0, SEEK_END);
    char dol = '$';
    int i = 0;
        // writting CidadeMae/Bebe string sizes
    #ifdef DEBUG
        printf("writting '%d'\t", reg->sizeCidadeMae);
    #endif
    fwrite(&reg->sizeCidadeMae, sizeof(int), 1 ,fp);
    #ifdef DEBUG
        printf("writting '%d'\n", reg->sizeCidadeBebe);
    #endif
    fwrite(&reg->sizeCidadeBebe, sizeof(int), 1 ,fp);
        // writting if size > 0
    if(reg->sizeCidadeMae)
    #ifdef DEBUG
        printf("writting '%s'\n", reg->cidadeMae);
    #endif
        fwrite(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp);
    if(reg->sizeCidadeBebe)
    #ifdef DEBUG
        printf("writting '%s'\n", reg->cidadeBebe);
    #endif
        fwrite(reg->cidadeBebe, sizeof(char), reg->sizeCidadeBebe, fp);
        // fill remaining reserved space with $
    while(i++< MAX_SIZE - (reg->sizeCidadeMae + reg->sizeCidadeBebe))
        fwrite(&dol, sizeof(char), 1, fp); 
        // writting remaining registry components 
    #ifdef DEBUG
        printf("writting '%d'\n", reg->idNascimento);
    #endif
    fwrite(&reg->idNascimento, sizeof(int), 1, fp);
    #ifdef DEBUG
        printf("writting '%d'\n", reg->idadeMae);
    #endif
    fwrite(&reg->idadeMae, sizeof(int), 1, fp);
    #ifdef DEBUG
        printf("writting '%s'\n", reg->dataNascimento);
    #endif
    fwrite(reg->dataNascimento, sizeof(char), NASC_SIZE, fp);
    #ifdef DEBUG
        printf("writting '%c'\n", reg->sexoBebe);
    #endif
    fwrite(&reg->sexoBebe, sizeof(char), 1, fp);
    #ifdef DEBUG
        printf("writting '%s'\n", reg->estadoMae);
    #endif
    fwrite(reg->estadoMae, sizeof(char), ESTADO_SIZE, fp);
    #ifdef DEBUG
        printf("writting '%s'\n", reg->estadoBebe);
    #endif
    fwrite(reg->estadoBebe, sizeof(char), ESTADO_SIZE, fp);
    
#ifdef DEBUG
    printf("\n\tOperation end.\n");
#endif

    return 0;
}
int bwrite_head(FILE *fp, HEAD *head){
        // saving pointer location
    long ft = ftell(fp);
    char dol = '$';
    int i = 0;
        // going to file first position
    fseek(fp, 0, SEEK_SET);
        // writting status byte
    fwrite(&head->status, sizeof(char), 1, fp);
        // writting other variables
    fwrite(&head->RRNproxRegistro, sizeof(int), 1, fp);
    fwrite(&head->numeroRegistrosInseridos, sizeof(int), 1, fp);
    fwrite(&head->numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fwrite(&head->numeroRegistrosAtualizado, sizeof(int), 1, fp);
        // fill file
    while(i++<HEAD_EMPTY_BYTES)
        fwrite(&dol, sizeof(char), 1, fp); 
        // returning fp to where it was
    fseek(fp, ft, SEEK_SET);
    return 0;
}
	// reading register from file
int fread_reg(FILE *fp, REG *reg ){
        //  easiest way - getline + strsep
    int i = 0;
    char *line = NULL, *lineaux = NULL, *aux = NULL;
    size_t size = GETLINE_RECOMMENDED_SIZE;
        //  reading entire line
    if(getline(&line, &size, fp) == -1 ){
        if (line != NULL) free(line);
        return 0;
    }
    line[strlen(line)-1] = '\0';
        // strsep overwrites target, so lets use a reference
    lineaux = line;    
        //  extracting tokens with strsep
    while((aux = strsep(&lineaux, ",")) != NULL){
    #ifdef DEBUG
        printf(" read '%s' of size %ld \n", aux, strlen(aux));
    #endif
        if(!strcmp(aux, "")){
            if(i == 0)
                reg->sizeCidadeMae = strlen(aux);
            else if(i == 1){
                reg->sizeCidadeBebe = strlen(aux);
            }else if(i == 3){
                reg->idadeMae = -1;
            }else if(i == 4){
                strncpy(reg->dataNascimento, "$$$$$$$$$$", NASC_SIZE);
                reg->dataNascimento[0] = '\0';
            }else if(i == 5){
                reg->sexoBebe = '\0';
            }else if(i == 6){
                strcpy(reg->estadoMae, "$$");
                reg->estadoMae[0] = '\0';
            }else{
                // i == 7
                strcpy(reg->estadoBebe, "$$");
                reg->estadoMae[0] = '\0';
            }
        }else{
            if(i == 0){
                reg->sizeCidadeMae = strlen(aux);
                strcpy(reg->cidadeMae, aux);
            }else if(i == 1){
                reg->sizeCidadeBebe = strlen(aux);
                strcpy(reg->cidadeBebe, aux);
            }else if(i == 2){
                reg->idNascimento = atoi(aux);
            }else if(i == 3){
                reg->idadeMae = atoi(aux);
            }else if(i == 4){
                strncpy(reg->dataNascimento, aux, NASC_SIZE);
            }else if(i == 5){
#ifdef DEBUG
    printf("baby sex: %s\n", aux);
#endif
                reg->sexoBebe = aux[0];
            }else if(i == 6){
                strncpy(reg->estadoMae, aux, ESTADO_SIZE);
            }else{
                // i == 7
                strncpy(reg->estadoBebe, aux, ESTADO_SIZE);
            }
        }
        i++;
    }
    free(line);
    return 0;
}
    // reading register from binary
int bread_reg(FILE *fp, REG *reg ){
    //stores position of pointer//
   int position= ftell(fp);
     // reads CidadeMae/Bebe string sizes
    fread(&reg->sizeCidadeMae, sizeof(int), 1 ,fp);
    fread(&reg->sizeCidadeBebe, sizeof(int), 1 ,fp);
    if(reg->sizeCidadeMae){
        fread(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp); 
        reg->cidadeMae[reg->sizeCidadeMae] = '\0';
    }
    if(reg->sizeCidadeBebe){
        fread(reg->cidadeBebe, sizeof(char), reg->sizeCidadeBebe, fp);
        reg->cidadeBebe[reg->sizeCidadeBebe] = '\0';
    }
//sets pointer to the begining of the rest of the variables
    fseek(fp, 0,SEEK_SET);
    position=position+105;
    fseek(fp, position,SEEK_SET);
  // fseek(fp, MAX_SIZE - (reg->sizeCidadeBebe + reg->sizeCidadeBebe), SEEK_CUR);
        // reading remaining registry components 
    fread(&reg->idNascimento, sizeof(int), 1, fp);
    fread(&reg->idadeMae, sizeof(int), 1, fp);
    fread(reg->dataNascimento, sizeof(char), NASC_SIZE, fp);
    fread(&reg->sexoBebe, sizeof(char), 1, fp);
    fread(reg->estadoMae, sizeof(char), ESTADO_SIZE, fp);
    fread(reg->estadoBebe, sizeof(char), ESTADO_SIZE, fp);

    return 0;
}


    // reading head from binary
int bread_head(FILE *fp, HEAD *head){
         // reading status byte
    fread(&head->status, sizeof(char), 1, fp);
    // reading other variables
    fread(&head->RRNproxRegistro, sizeof(int), 1, fp);
    fread(&head->numeroRegistrosInseridos, sizeof(int), 1, fp);
    fread(&head->numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fread(&head->numeroRegistrosAtualizado, sizeof(int), 1, fp);
//points to the beggining of the registers//
    fseek(fp, 128, SEEK_SET);

    return 0;
}

int print_reg(REG *reg){
    
        //goes through every reg printing them//
        //checks if it wasn't removed//
    if(reg->cidadeMae >= 0 ){

        //checks if there's a valid answer and prints the variable//
        if(reg->sizeCidadeBebe > 0){
            printf("Nasceu em: %s", reg->cidadeBebe);
        }
        else{
            printf("Nasceu em: -");
        }

        if(reg->estadoBebe[0] != '\0'){
            printf("/%s, em ", reg->estadoBebe);
        }
        else{
            printf("/-, em ");
        }
        if(reg->dataNascimento[0] != '\0'){
            printf("%s, ", reg->dataNascimento);
        }
        else{
            printf("-, ");
        }

        if(reg->sexoBebe == '1'){
            printf("um bebe de sexo MASCULINO.\n");
        }else if(reg->sexoBebe == '2'){
            printf("um bebe de sexo FEMININO.\n");
        }else{ //(reg->sexoBebe == 0)
            printf("um bebe de sexo IGNORADO.\n");
        }

       return 1; 
    }
    return 0;
}

int mfeof(FILE *fp) {
        long cur = ftell(fp), max;

        fseek(fp, 0, SEEK_END);
        max = ftell(fp);

        fseek(fp, cur, SEEK_SET);
                    
        if(cur < max)
            return 0;
        return 1;
}
