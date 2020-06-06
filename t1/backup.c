#include"crud.h"

    // writting registry to binary
int bwrite_reg(FILE *fp, REG *reg){
    char dol = '$';
    int i = 0;
#ifdef DEBUG
    printf("writting '%d'\t", reg->sizeCidadeMae);
    printf("writting '%d'\n", reg->sizeCidadeBebe);
#endif
        // writting CidadeMae/Bebe string sizes
    fwrite(&reg->sizeCidadeMae, sizeof(int), 1 ,fp);
    fwrite(&reg->sizeCidadeBebe, sizeof(int), 1 ,fp);
        
        // writting if string cidadeMae exists 
    if(reg->sizeCidadeMae)
#ifdef DEBUG
    printf("writting '%s'\n", reg->cidadeMae);
#endif
        fwrite(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp);

        // writting if string cidadeBebe exists 
    if(reg->sizeCidadeBebe)
#ifdef DEBUG
    printf("writting '%s'\n", reg->cidadeBebe);
#endif
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
    
#ifdef DEBUG
    printf("writting '%d'\n", reg->idNascimento);
    printf("writting '%d'\n", reg->idadeMae);
    printf("writting '%s'\n", reg->dataNascimento);
    printf("writting '%c'\n", reg->sexoBebe);
    printf("writting '%s'\n", reg->estadoMae);
    printf("writting '%s'\n", reg->estadoBebe);
    printf("\n\tOperation end.\n");
#endif

    return 0;
}
    // writting header to binary
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

        // fill remaining reserved space with $
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
            // if getline fails, stop operation here
        if (line != NULL) free(line);
        return 0;
    }
        // removing '\n' from string end
    line[strlen(line)-1] = '\0';

        /* strsep overwrites target, so lets use a reference
         * using line itself will lead to memory leak. */
    lineaux = line;    
        //  extracting tokens with strsep
        //      using ',' as delimiter
    while((aux = strsep(&lineaux, ",")) != NULL){
#ifdef DEBUG
    printf(" read '%s' of size %ld \n", aux, strlen(aux));
#endif
            // dealing with null values
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
                reg->sexoBebe = '0';
            }else if(i == 6){
                strcpy(reg->estadoMae, "$$");
                reg->estadoMae[0] = '\0';
            }else{
                strcpy(reg->estadoBebe, "$$");
                reg->estadoBebe[0] = '\0';
            }
        }else{
            // saving read data to struct
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
                reg->sexoBebe = aux[0];
            }else if(i == 6){
                strncpy(reg->estadoMae, aux, ESTADO_SIZE);
            }else{
                strncpy(reg->estadoBebe, aux, ESTADO_SIZE);
            }
        }
            // i counts which column i'm currently working on
        i++;
    }
    free(line);
    return 0;
}
    // reading register from binary
int bread_reg(FILE *fp, REG *reg ){
        // stores position of pointer
    int position= ftell(fp);
        // reads CidadeMae/Bebe string sizes
    fread(&reg->sizeCidadeMae, sizeof(int), 1 ,fp);
    fread(&reg->sizeCidadeBebe, sizeof(int), 1 ,fp);

        // read if string cidadeMae exists 
    if(reg->sizeCidadeMae){
        fread(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp); 
        reg->cidadeMae[reg->sizeCidadeMae] = '\0';
    }
        // read if string cidadeBebe exists 
    if(reg->sizeCidadeBebe){
        fread(reg->cidadeBebe, sizeof(char), reg->sizeCidadeBebe, fp);
        reg->cidadeBebe[reg->sizeCidadeBebe] = '\0';
    }
        // sets pointer to the begining of the rest of the variables
    fseek(fp, 0,SEEK_SET);
    position += VAR_SIZE_OFFSET;
    fseek(fp, position,SEEK_SET);

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
        // points to the beggining of the registers
    fseek(fp, SIZEOF_REG, SEEK_SET);

    return 0;
}

int print_reg(REG *reg){
    
        // checks if it wasn't removed
    if(reg->cidadeMae >= 0 ){

            // checks if there's a string and prints the variable
        if(reg->sizeCidadeBebe > 0){
            printf("Nasceu em %s", reg->cidadeBebe);
        }
        else{
            printf("Nasceu em -");
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
            printf("um bebê de sexo MASCULINO.\n");
        }else if(reg->sexoBebe == '2'){
            printf("um bebê de sexo FEMININO.\n");
        }else{ //(reg->sexoBebe == 0)
            printf("um bebê de sexo IGNORADO.\n");
        }

       return 1; 
    }
    return 0;
}



int update_field(char *strType, char *strContent, FILE *b){
        REG reg = {0};   
        char dol = '$', one = '1', two = '2';
        int NextField = ftell(b) + 105;
        int aux=-1, bo,  null = 0, number=0, zero=0;
        int compare=0;
        printf("%d\n", compare);
        int kkk= ftell(b);
        int CityLength;
        int bbb;
        //checks which one is the field to be updated

        //IDADE DO BEBE/
         compare = strcmp(strType, "idNascimento");
        if(!compare){

            //moves pointer to right position//
            fseek(b, 105,SEEK_CUR);

            
            //checks the gender and writes in the proper place
            compare=strcmp(strContent, "MASCULINO");
            if(!compare){
                fwrite(&one,sizeof(char),1,b);
            }
            compare=strcmp(strContent, "FEMININO");
            if(!compare){
                fwrite(&two,sizeof(char),1,b);
            }
        }
            //IDADE MAE//
        compare = strcmp(strType, "idadeMae");
        if(!compare){
            //moves pointer to right position//
            fseek(b, 109,SEEK_CUR);
            //casts content to integer
            number=atoi(strContent);

             //checks if the value altered is NULL//
           compare = strcmp(strContent, "NULO");
            if(!compare){
                fwrite(&null,sizeof(int),1,b);
            } else fwrite(&number,sizeof(int),1,b);
        }
                  
        //SEXO BEBE//
        compare = strcmp(strType, "sexoBebe");
        if(!compare){

            //moves pointer to right position//
            fseek(b, 123,SEEK_CUR);

             //checks if the value altered is NULL//
           compare = strcmp(strContent, "NULO");
            if(!compare){
                fwrite(&null,sizeof(char),1,b);
            }

            //checks the gender and writes in the proper place
            compare=strcmp(strContent, "MASCULINO");
            if(!compare){
                fwrite(&one,sizeof(char),1,b);
            }
            compare=strcmp(strContent, "FEMININO");
            if(!compare){
                fwrite(&two,sizeof(char),1,b);
            }
        }
          /*  bo = ftell(b);
            printf("%d dentro\n", ftell(b));
*/
        //nascimento bebe//
        compare = strcmp(strType, "dataNascimento");
        printf("%d data \n", compare);
        if(!compare){

            //moves pointer to right position//
            fseek(b, 113,SEEK_CUR);
            kkk= ftell(b);
            printf("%d dentro \n", kkk);
             //checks if the value altered is NULL//
           compare = strcmp(strContent, "NULO");
            if(!compare){
                fwrite(&null,sizeof(char),1,b);
            } else fwrite(strContent, sizeof(char), 10, b);
        }

           //ESTADO MAE
        compare = strcmp(strType, "estadoMae");
        printf("%d estado \n", compare);
        if(!compare){

            //moves pointer to right position//
            fseek(b, 124,SEEK_CUR);
            bo=ftell(b);
            
             //checks if the value altered is NULL//
           compare = strcmp(strContent, "NULO");
            if(!compare){
                fwrite(&null,sizeof(char),2,b);
            } else fwrite(strContent, sizeof(char), 2, b);
        }
        
         //ESTADO BEBE
        compare = strcmp(strType, "estadoBebe");
        printf("%d estado \n", compare);
        if(!compare){

            //moves pointer to right position//
            fseek(b, 126,SEEK_CUR);
            bo=ftell(b);
            
             //checks if the value altered is NULL//
           compare = strcmp(strContent, "NULO");
            if(!compare){
                fwrite(&null,sizeof(char),2,b);
            } else fwrite(strContent, sizeof(char), 2, b);
        }


          ///CIDADE MAE// 
         compare = strcmp(strType, "cidadeMae");
        printf("%d CidadeMae \n", compare);
        kkk=ftell(b);
            printf("%d\n", kkk);
        if(!compare){
            int position =ftell(b);
            NextField= position +105;
            kkk=ftell(b);
            printf("%d\n", kkk);

             //saves the register
            bread_reg(b, &reg);
            kkk=ftell(b);
            printf("%d\n", kkk);

            //moves pointer to right position//
            fseek(b, -120,SEEK_CUR);

            bbb =ftell(b);
             //checks if the value altered is NULL//
           compare = strcmp(strContent, "NULO");
            if(!compare){
                fwrite(reg.cidadeBebe,sizeof(char),reg.sizeCidadeBebe,b);

                //checks if the rest is checked with $
                char i= fread(&i, sizeof(char),1, b);
                while(i != dol && ftell(b) < NextField){
                fseek(b, -1,SEEK_CUR);
                fwrite(&dol, sizeof(char), 1, b);
                i= fread(&i, sizeof(char),1, b);


                //muda o valor campo sizecidadeMae 
            fseek(b, position, SEEK_SET);
            fwrite(&zero,sizeof(int), 1, b);
            }
            

            } else{
                fseek(b, position, SEEK_SET);
                //checks size of string and saves it
                CityLength = strlen(strContent);
                fwrite(&CityLength,sizeof(int), 1, b);

                //writes both cities 
                fseek(b, 4, SEEK_CUR);
                fwrite(strContent, sizeof(char), CityLength, b);
                fwrite(reg.cidadeBebe,sizeof(char),reg.sizeCidadeBebe,b);

                //checks if the rest is checked with $
                char i= fread(&i, sizeof(char),1, b);
                while(i != dol && ftell(b) < NextField){
                fseek(b, -1,SEEK_CUR);
                fwrite(&dol, sizeof(char), 1, b);
                i= fread(&i, sizeof(char),1, b);

                } 
            }
        }
            
            
        //CIDADE DO BEBE//
        compare = strcmp(strType, "cidadeBebe");
        printf("%d cidadeBebe \n", compare);
        if(!compare){
            int position =ftell(b);
            NextField= position +105;
            kkk=ftell(b);
            printf("%d\n", kkk);

             //saves the register
            bread_reg(b, &reg);
            kkk=ftell(b);
            printf("%d\n", kkk);

            //moves pointer to right position//
            fseek(b, -128,SEEK_CUR);

            
             //checks if the value altered is NULL//
           compare = strcmp(strContent, "NULO");
            if(!compare){

                //goes to the place where the child's bith place was stored
                fseek(b, reg.sizeCidadeBebe, SEEK_CUR); 

                //checks if the rest is checked with $
                char i= fread(&i, sizeof(char),1, b);
                while(i != dol && ftell(b) < NextField){
                fseek(b, -1,SEEK_CUR);
                fwrite(&dol, sizeof(char), 1, b);
                i= fread(&i, sizeof(char),1, b);

                //changes size of birth place to NULL
                fseek(b, position+4, SEEK_SET);
                fwrite(&zero,sizeof(int),1,b);
            }

            } else{
                //goes to the field sizeCidadeBeb
                fseek(b, position+4, SEEK_SET);

                //checks size of string and saves it
                CityLength = strlen(strContent);
                fwrite(&CityLength,sizeof(int), 1, b);

                //writes city 
                fseek(b, reg.sizeCidadeMae, SEEK_CUR);
                bbb =ftell(b);
                fwrite(strContent, sizeof(char), CityLength, b);
            

                //checks if the rest is checked with $
                char i= fread(&i, sizeof(char),1, b);
                while(i != dol && ftell(b) < NextField){
                fseek(b, -1,SEEK_CUR);
                fwrite(&dol, sizeof(char), 1, b);
                i= fread(&i, sizeof(char),1, b);

                } 
            }
        }

/*
fseek(b,bbb, SEEK_SET);
char str[CityLength];
printf("%d\n", bbb);
fread(str, sizeof(char), CityLength, b);
printf("%s amazenado", str);
*/
return 0;

}

/*void bwrite_reg(FILE *fp){
         // stores position of pointer
    int position= ftell(fp);
        // reads CidadeMae/Bebe string sizes
    fwrite(&reg->sizeCidadeMae, sizeof(int), 1 ,fp);
    fwrite(&reg->sizeCidadeBebe, sizeof(int), 1 ,fp);

        // read if string cidadeMae exists 
    if(reg->sizeCidadeMae){
        fread(reg->cidadeMae, sizeof(char), reg->sizeCidadeMae, fp); 
        reg->cidadeMae[reg->sizeCidadeMae] = '\0';
    }
        // read if string cidadeBebe exists 
    if(reg->sizeCidadeBebe){
        fread(reg->cidadeBebe, sizeof(char), reg->sizeCidadeBebe, fp);
        reg->cidadeBebe[reg->sizeCidadeBebe] = '\0';
    }
        // sets pointer to the begining of the rest of the variables
    fseek(fp, 0,SEEK_SET);
    position += VAR_SIZE_OFFSET;
    fseek(fp, position,SEEK_SET);

        // reading remaining registry components 
    fread(&reg->idNascimento, sizeof(int), 1, fp);
    fread(&reg->idadeMae, sizeof(int), 1, fp);
    fread(reg->dataNascimento, sizeof(char), NASC_SIZE, fp);
    fread(&reg->sexoBebe, sizeof(char), 1, fp);
    fread(reg->estadoMae, sizeof(char), ESTADO_SIZE, fp);
    fread(reg->estadoBebe, sizeof(char), ESTADO_SIZE, fp);

    return 0;
}
void atualize_reg(){

}
*/

int mfeof(FILE *fp) {
        // feof that works
    long cur = ftell(fp), max;

    fseek(fp, 0, SEEK_END);
    max = ftell(fp);

    fseek(fp, cur, SEEK_SET);
        // """"""compares SEEK_CUR with EOF""""""
    if(cur < max)
        return 0;
    return 1;
}
