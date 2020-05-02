#include"crud.h"

    // writting registry/header to binary
int bwrite_reg(FILE *fp, REG *reg){

    fwrite(reg->sizeCidadeMae, sizeof(int), 1 ,fp);
    fwrite(reg->sizeCidadeBebe, sizeof(int), 1 ,fp);

    if(reg->sizeCidadeMae)
        fwrite(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp);
    if(reg->sizeCidadeBebe)
        fwrite(reg->cidadeBebe, sizeof(char), reg->sizeCidadeBebe, fp);
        // fill with $
    fwrite("$", sizeof(char), MAX_SIZE-(reg->sizeCidadeMae + reg->sizeCidadeBebe), fp); 
        // other 
    fwrite(reg->idNascimento, sizeof(int), 1, fp);
    fwrite(reg->idadeMae, sizeof(int), 1, fp);
    fwrite(reg->dataNascimento, sizeof(char), NASC_SIZE, fp);
    fwrite(reg->sexoBebe, sizeof(char), 1, fp);
    fwrite(reg->estadoMae, sizeof(char), ESTADO_SIZE, fp);
    fwrite(reg->estadoBebe, sizeof(char), ESTADO_SIZE, fp);
        // come back later
    return 0;
}
int bwrite_head(FILE *fp, HEAD *head){
        // saving pointer location
    long ft = ftell(fp);
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
    fwrite("$", sizeof(char), HEAD_EMPTY_BYTES, fp); 
        // returning fp to where it was
    fseek(fp, ft, SEEK_SET);
    return 0;
}
	// reading register from file
int fread_reg(FILE *fp, REG *reg ){
        //  easiest way - getline + strsep
    char *line = NULL, *aux = NULL;
    size_t size = GETLINE_RECOMMENDED_SIZE;
        //  reading entire line
    getline(&line, &size, fp);
        //  extracting tokens with strsep
    
    free(line);
    return 0;
}
	// reading register from binary
int bread_reg(FILE *fp, REG *reg ){
    return 0;
}
	// miscellanea
int print_reg(REG *reg){
    return 0;
}

int erase_reg(REG *reg){
    return 0;
}


