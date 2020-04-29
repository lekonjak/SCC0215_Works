#ifndef _CRUD_H_
#define _CRUD_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct reg REG;
typedef struct head HEAD;
	// writting registry/header to binary
int bwrite_reg(FILE *fp, REG *reg);
int bwrite_head(FILE *fp, HEAD *head);
	// reading register from file
int fread_reg(FILE *fp);
	// reading register from binary
int bread_reg(FILE *fp);
	// miscellanea
int print_reg(REG *reg);

int erase_reg(REG *reg);

#endif
