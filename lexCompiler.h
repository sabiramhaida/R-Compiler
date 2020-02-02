#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum{
	BOOLEAN_TOKEN,
	REPEAT_TOKEN,
	WHILE_TOKEN ,
	FUNC_TOKEN ,
	RET_TOKEN ,
	IF_TOKEN ,
	ELSE_TOKEN,
	FOR_TOKEN ,
	IN_TOKEN,
	LIST_TOKEN,
	C_TOKEN,
	ID_TOKEN,
	STRING_TOKEN,
	NUMBER_TOKEN,
	EOF_TOKEN,
	OP_TOKEN ,
	CP_TOKEN ,
	OCB_TOKEN,
	CCB_TOKEN,
	COMMA_TOKEN,
	OSB_TOKEN,
	CSB_TOKEN,
	EXCLAMATION_TOKEN,
	DOLLAR_TOKEN,
	DOT_TOKEN,
	COMMENT_TOKEN,
	SEMICOLON_TOKEN,
	NOT_TOKEN,
	PLUS_TOKEN,
	MINUS_TOKEN,
	MULT_TOKEN ,
	DIV_TOKEN ,
	GT_TOKEN ,
	LT_TOKEN ,
	GE_TOKEN,
	LE_TOKEN,
	EQ_TOKEN,
	DIFF_TOKEN,
	OR_TOKEN ,
	AND_TOKEN,
	POW_TOKEN,
	AFF_TOKEN,
	COLON_TOKEN,
	ERROR_TOKEN
}CODE_LEX;

//char* nom = calloc(1028,sizeof(char));
typedef struct {
	CODE_LEX CODE;
	char nom[];
}TcurrentSymbole;

//Déclaration Variable Globale
TcurrentSymbole currentSymbole;
char currentCaractere;
char Car_suivant;
FILE* file;

//Déclaration Prototype des méthodes
void readCaractere();
void readWord();
void readNumber();
void readComment();
void nextSymbole();
void openFile(char* fileName);
void printToken(TcurrentSymbole sym);
int AlphaNum(char currentCaractere);
int IgnoredCaracters(char currentCaractere);
int CaractereSigne(char currentCaractere);
