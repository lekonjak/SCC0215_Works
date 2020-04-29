#ifndef CRUD_H
#define CRUD_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>

#define MAX_SIZE 97
#define ESTADO_SIZE 2
#define GETLINE_RECOMMENDED_SIZE 120
#define SIZEOF_REG 128

typedef struct reg{
	int sizeCidadeMae, sizeCidadeBb, idNascimento, idadeMae, dataNasc, sexoBebe;
	char cidadeMae[MAX_SIZE], cidadeBebe[MAX_SIZE], estadoMae[ESTADO_SIZE], estadoBebe[ESTADO_SIZE];
}REG;

typedef struct head{
	char status;
	int RRNproxRegistro, numeroRegistrosInseridos, numeroRegistrosRemovidos, numeroRegistrosAtualizado;
}HEAD;

    // writting registry/header to binary
int bwrite_reg(FILE *fp, REG *reg);
int bwrite_head(FILE *fp, HEAD *head);
	// reading register from FILE
int fread_reg(FILE *fp, REG *reg);
	// reading register from binary
int bread_reg(FILE *fp, REG *reg);
	// miscellanea
int print_reg(REG *reg);

int erase_reg(REG *reg);

#endif
