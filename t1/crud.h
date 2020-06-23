#ifndef CRUD_H
#define CRUD_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 97
#define VAR_SIZE_OFFSET 105
#define ESTADO_SIZE 2
#define NASC_SIZE 10
#define GETLINE_RECOMMENDED_SIZE 120
#define SIZEOF_REG 128
#define HEAD_EMPTY_BYTES 111

typedef struct reg{
    char sexoBebe;
	int sizeCidadeMae, sizeCidadeBebe, idNascimento, idadeMae;
	char cidadeMae[MAX_SIZE], cidadeBebe[MAX_SIZE], estadoMae[ESTADO_SIZE+1], estadoBebe[ESTADO_SIZE+1], dataNascimento[NASC_SIZE+1];
}REG;

typedef struct head{
	char status;
	int RRNproxRegistro, numeroRegistrosInseridos, numeroRegistrosRemovidos, numeroRegistrosAtualizado;
}HEAD;

typedef struct btree_head{
	char status;
	int noRaiz, nroNiveis, proxRRN, nroChaves;
}BTREE_HEAD;

typedef struct btree_reg{
	int nivel, n, C1, Pr1, C2, Pr2, C3, Pr3, C4, Pr4, C5, Pr5, P1, P2, P3, P4, P5, P6;
}BTREE_REG;

    	// writting registry/header to binary
int bwrite_reg(FILE *fp, REG *reg);
int bwrite_head(FILE *fp, HEAD *head);
	// reading register from FILE
int fread_reg(FILE *fp, REG *reg);
	// reading register from binary
int bread_reg(FILE *fp, REG *reg);
	// reading head from binary
int bread_head(FILE *fp, HEAD *head);
	// miscellanea
int print_reg(REG *reg);
   	// our feof implementation
int mfeof(FILE *fp);
	// remove quotes from strings
int quotes_clean(char *c);
	// to handle with quoted sentences with spaces using strtok
int space_converter(char *c);
	// to convert back a sentence converted by space_converter
int space_return(char *c);

int update_field(char *strType, char *strContent, FILE *b, int tmp);

int update_rrn(char *out, char *bin);
	// writting btree registry/header to binary
int btwrite_head(FILE *fp, BTREE_HEAD *head);
#endif
