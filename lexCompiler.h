#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef enum{
ID_TOKEN,STRING_TOKEN,BOOLEAN_TOKEN,EOF_TOKEN ,FUNC_TOKEN ,OP_TOKEN ,CP_TOKEN ,OCB_TOKEN ,CCB_TOKEN,COMMA_TOKEN,OSB_TOKEN,CSB_TOKEN,EXCLAMATION_TOKEN,
DOLLAR_TOKEN,C_TOKEN,LIST_TOKEN,NUMBER_TOKEN,DOT_TOKEN,UNDERSCORE_TOKEN,PLUS_TOKEN,MINUS_TOKEN,MULT_TOKEN,DIV_TOKEN,
GT_TOKEN,LT_TOKEN ,GE_TOKEN ,LE_TOKEN ,EQ_TOKEN ,DIFF_TOKEN,OR_TOKEN,AND_TOKEN,POW_TOKEN,AFF_TOKEN,RET_TOKEN,IF_TOKEN,
ELSE_TOKEN,FOR_TOKEN,IN_TOKEN,COLON_TOKEN,REPEAT_TOKEN,WHILE_TOKEN,ERROR_TOKEN,SEMICOLON_TOKEN,COMMENT_TOKEN,PRINT_TOKEN
}CODE_LEX;

//char* nom = calloc(1028,sizeof(char));
typedef struct {
	CODE_LEX CODE;
	char nom[20];
}TcurrentSymbole;

//Déclaration Variable Globale
TcurrentSymbole currentSymbole;
char currentCaractere;
char Car_suivant;
FILE* file;
FILE* fileprint;
int fl=0;


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
