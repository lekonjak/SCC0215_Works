#include"crud.h"
#define sizeofreg 128

struct reg{
	int sizeCidadeMae, sizeCidadeBb, idNascimento, idadeMae, dataNasc, sexoBebe, estadoMae, estadoBebe;
	char cidadeMae[97], cidadeBebe[97];
};

struct head{
	char status;
	int RRNproxRegistro, numeroRegistrosInseridos, numeroRegistrosRemovidos, numeroRegistrosAtualizado;
};

int fwrite_reg(FILE *fp, REG *reg){
	return 0;
}

int fread_reg(FILE *fp){
		return 0;
}

int print_reg(REG *reg){
		return 0;
}

int erase_reg(REG *reg){
	/*
	 *	Libero aqui as partes da estrutura de dados utilizada.
	 * */
	free(reg);
	return 0;	
}


