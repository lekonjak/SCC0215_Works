#include"crud.h"

    // writting registry/header to binary
int bwrite_reg(FILE *fp, REG *reg){
    char dol = '$';
    int i = 0;
        // writting CidadeMae/Bebe string sizes
    fwrite(&reg->sizeCidadeMae, sizeof(int), 1 ,fp);
    fwrite(&reg->sizeCidadeBebe, sizeof(int), 1 ,fp);
        // writting if size > 0
    if(reg->sizeCidadeMae)
        fwrite(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp);
    if(reg->sizeCidadeBebe)
        fwrite(reg->cidadeBebe, sizeof(char), reg->sizeCidadeBebe, fp);
        // fill remaining reserved space with $
    while(i++< MAX_SIZE - (reg->sizeCidadeMae + reg->sizeCidadeBebe))
        fwrite(&dol, sizeof(char), 1, fp); 
        // writting remaining registry components 
    fwrite(&reg->idNascimento, sizeof(int), 1, fp);
    fwrite(&reg->idadeMae, sizeof(int), 1, fp);
    fwrite(reg->dataNascimento, sizeof(char), NASC_SIZE, fp);
    fwrite(&reg->sexoBebe, sizeof(char), 1, fp);
    fwrite(reg->estadoMae, sizeof(char), ESTADO_SIZE, fp);
    fwrite(reg->estadoBebe, sizeof(char), ESTADO_SIZE, fp);
        // come back later
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
    char *line = NULL, *aux = NULL;
    size_t size = GETLINE_RECOMMENDED_SIZE;
        //  reading entire line
    if(getline(&line, &size, fp)<0){
        if (aux != NULL) free(aux);
        return 0;
    }
        
        //  extracting tokens with strsep
    while((aux = strsep(&line, ",")) != NULL){
        if(!strcmp(aux, "")){
            if(i == 0)
                reg->sizeCidadeMae = strlen(aux);
            else if(i == 1){
                reg->sizeCidadeBebe = strlen(aux);
            }else if(i == 3){
                reg->idadeMae = -1;
            }else if(i == 4){
                strcpy(reg->dataNascimento, "\0$$$$$$$$$");
            }else if(i == 5){
                reg->sexoBebe = '\0';
            }else if(i == 6){
                strcpy(reg->estadoMae, "\0$");
            }else{
                // i == 7
                strcpy(reg->estadoBebe, "\0$");
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
                strcpy(reg->dataNascimento, aux);
            }else if(i == 5){
                reg->sexoBebe = aux[0];
            }else if(i == 6){
                strcpy(reg->estadoMae, aux);
            }else{
                // i == 7
                strcpy(reg->estadoBebe, aux);
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

    fread(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp); 
    fread(reg->cidadeBebe, sizeof(char), reg->sizeCidadeBebe, fp);
//sets pointer to the begining of the rest of the variables
    fseek(fp, 0,SEEK_SET);
    position=position+105;
    fseek(fp, position,SEEK_SET);
    
        // writting remaining registry components 
    fread(&reg->idNascimento, sizeof(char), 10, fp);
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

int print_reg(REG *reg, int numeroRegistrosInseridos, FILE *fileP){
    
        //goes through every reg printing them//
    for(int i=0; i < numeroRegistrosInseridos; i++){
        bread_reg(fileP, reg);
        //checks if it wasn't removed//
        if(reg->cidadeMae >= 0 ){

        //checks if there's a valid answer and prints the variable//
            if(reg->cidadeBebe[0] != '\0'){
                printf("Nasceu em: %s, ", reg->cidadeBebe);
            }
            else{
                printf("Nasceu em: -, ");
            }

            if(reg->estadoBebe[0] != '\0'){
                printf("/%s, ", reg->estadoBebe);
            }
             else{
                printf("/-, ");
            }
            if(reg->dataNascimento[0] != '$'){
                printf("%s, ", reg->dataNascimento);
            }
            else{
                printf("-, ");
            }

            if(reg->sexoBebe == 1){
                printf("um bebe de sexo MASCULINO.\n");
            }
            if(reg->sexoBebe == 2){
                printf("um bebe de sexo FEMININO.\n");
            }
            if(reg->sexoBebe == 0){
                printf("um bebe de sexo IGNORADO.\n");
            }
           
        }
    }

    return 0;

        //erro//
    printf("Falha no processamento do arquivo");
}

int mfeof(FILE *fp) {
        long cur = ftell(fp), max;

        fseek(fp, 0, SEEK_END);
        max = ftell(fp);

        fseek(fp, cur, SEEK_SET);
                    
        if(ftell(fp) < max)
            return 0;
        return 1;
}
