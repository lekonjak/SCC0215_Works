#include"crud.h"

    // writting registry/header to binary
int bwrite_reg(FILE *fp, REG *reg){
        // writting CidadeMae/Bebe string sizes
    fwrite(&reg->sizeCidadeMae, sizeof(int), 1 ,fp);
    fwrite(&reg->sizeCidadeBebe, sizeof(int), 1 ,fp);
        // writting if size > 0
    if(reg->sizeCidadeMae)
        fwrite(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp);
    if(reg->sizeCidadeBebe)
        fwrite(reg->cidadeBebe, sizeof(char), reg->sizeCidadeBebe, fp);
        // fill remaining reserved space with $
    fwrite("$", sizeof(char), MAX_SIZE-(reg->sizeCidadeMae + reg->sizeCidadeBebe), fp); 
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
    getline(&line, &size, fp);
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
int bread_reg(FILE *fp, REG *reg){
    return 0;
}
    // reading head from binary
int bread_head(FILE *fp, HEAD *head){
    return 0;
}
	// miscellanea
int print_reg(REG *reg){
    return 0;
}
