#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lexCompiler.h"

void nextSymbole(){
	while(IgnoredCaracters(currentCaractere)){
		readCaractere();
	}
	if((currentCaractere>='a' && currentCaractere<='z')||(currentCaractere>='A' && currentCaractere<='Z')) readWord();
	
	else if (currentCaractere=='.'){
		Car_suivant= fgetc(file);
		if ((Car_suivant>='a' && Car_suivant<='z')||(Car_suivant>='A' && Car_suivant<='Z')){
		readWord();
		}
	}
	else if(currentCaractere>='0' && currentCaractere<='9'){
		readNumber();
	}
	else if(currentCaractere=='#'){
		readComment();
	}
	else{
		int _compt=0;
		char* mot = calloc(1028,sizeof(char));
		*(mot+_compt)=currentCaractere;
		switch(currentCaractere){
			case '+': 
				currentSymbole.CODE=PLUS_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '-': 
				currentSymbole.CODE=MINUS_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '*': 
				currentSymbole.CODE=MULT_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '/': 
				currentSymbole.CODE=DIV_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '.': 
				currentSymbole.CODE=DOT_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case ',': 
				currentSymbole.CODE=COMMA_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case ';': 
				currentSymbole.CODE=SEMICOLON_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '(': 
				currentSymbole.CODE=OP_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case ')': 
				currentSymbole.CODE=CP_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '{': 
				currentSymbole.CODE=OCB_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '}': 
				currentSymbole.CODE=CCB_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '[': 
				currentSymbole.CODE=OSB_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case ']': 
				currentSymbole.CODE=CSB_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '|': 
				currentSymbole.CODE=OR_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '&': 
				currentSymbole.CODE=AND_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '^': 
				currentSymbole.CODE=POW_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '$': 
				currentSymbole.CODE=DOLLAR_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
			case '=':
                readCaractere();			
				if(currentCaractere=='=') {
					_compt++;
					*(mot+_compt)=currentCaractere;
					currentSymbole.CODE=EQ_TOKEN;
					strcpy(currentSymbole.nom,mot);
					readCaractere();
				}
				else {
					currentSymbole.CODE=AFF_TOKEN;
					strcpy(currentSymbole.nom,mot);
				}
				break;
			case '>': 
				readCaractere();
				if(currentCaractere=='=') {
					_compt++;
					*(mot+_compt)=currentCaractere;
					currentSymbole.CODE=GE_TOKEN;
					strcpy(currentSymbole.nom,mot);
					readCaractere();
				}
				else{
				currentSymbole.CODE=GT_TOKEN;
				strcpy(currentSymbole.nom,mot);
				}
				break;
			case '<': 
				readCaractere();
				if(currentCaractere=='=') {
					_compt++;
					*(mot+_compt)=currentCaractere;
					currentSymbole.CODE=LE_TOKEN;
					strcpy(currentSymbole.nom,mot);
					readCaractere();
				}
				else if(currentCaractere=='-') {
					_compt++;
					*(mot+_compt)=currentCaractere;
					currentSymbole.CODE=AFF_TOKEN;
					strcpy(currentSymbole.nom,mot);
					readCaractere();
				}
				else{
				currentSymbole.CODE=LT_TOKEN;
				strcpy(currentSymbole.nom,mot);
				}
				break;
			case '!': 
				readCaractere();
				if(currentCaractere=='=') {
					_compt++;
					*(mot+_compt)=currentCaractere;
					currentSymbole.CODE=DIFF_TOKEN;
					strcpy(currentSymbole.nom,mot);
					readCaractere();
				}
				else{
				currentSymbole.CODE=EXCLAMATION_TOKEN;
				strcpy(currentSymbole.nom,mot);
				}
				break;
			case ':':
				currentSymbole.CODE=COLON_TOKEN;
				strcpy(currentSymbole.nom,mot);
				readCaractere();
				break;
				// on considere pas le cas d'erreur
				break;
			case EOF: 
				currentSymbole.CODE=EOF_TOKEN;
				strcpy(currentSymbole.nom,"EOF_TOKEN");
				break;
			default :
				currentSymbole.CODE=ERROR_TOKEN;
				strcpy(currentSymbole.nom,"ERROR_TOKEN");
				while(!IgnoredCaracters(currentCaractere)){
				readCaractere();
				}
		}
	}
	
}

void readCaractere(){
	currentCaractere=fgetc(file);
}

void readWord(){
	int _compt=0;
	char* mot = calloc(1028,sizeof(char));
	*(mot+_compt)=currentCaractere;
	if (currentCaractere=='.') {
		 _compt++;
		*(mot+_compt)=Car_suivant;
	}

	readCaractere();
	while(AlphaNum(currentCaractere)){
		_compt++;
		*(mot+_compt)=currentCaractere;
		readCaractere();
	}
	*(mot+_compt+1)='\0';
	if(!strcmp(mot,"TRUE")){
		currentSymbole.CODE=BOOLEAN_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"FALSE")){
		currentSymbole.CODE=BOOLEAN_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"while")){
		strcpy(currentSymbole.nom,mot);
		currentSymbole.CODE=WHILE_TOKEN;
	}else if(!strcmp(mot,"function")){
		currentSymbole.CODE=FUNC_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"return")){
		currentSymbole.CODE=RET_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"if")){
		currentSymbole.CODE=IF_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"else")){
		currentSymbole.CODE=ELSE_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"repeat")){
		currentSymbole.CODE=REPEAT_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"for")){
		currentSymbole.CODE=FOR_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"in")){
		currentSymbole.CODE=IN_TOKEN ;
		strcpy(currentSymbole.nom,mot);
	}else if(!strcmp(mot,"list")){
		currentSymbole.CODE=LIST_TOKEN ;
		strcpy(currentSymbole.nom,mot);
	}else{
		currentSymbole.CODE=ID_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}
	memset(mot,'\0',1024);
} 

void readNumber(){
	int _compt=0;
	char* mot = calloc(1028,sizeof(char));
	while(currentCaractere>='0' && currentCaractere<='9'){
	*(mot+_compt)=currentCaractere;
		_compt++;
		readCaractere();
	}
	*(mot+_compt+1)='\0';
	if(IgnoredCaracters(currentCaractere) || CaractereSigne(currentCaractere)){
		currentSymbole.CODE = NUMBER_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}else{
		while(!IgnoredCaracters(currentCaractere))
			readCaractere();
		currentSymbole.CODE = ERROR_TOKEN;
		strcpy(currentSymbole.nom,mot);
	}
	memset(mot,'\0',1024);
}

void readComment(){
	readCaractere();
	while(currentCaractere!='\n' && currentCaractere!=EOF){
		readCaractere();
	}
	currentSymbole.CODE = COMMENT_TOKEN;
	strcpy(currentSymbole.nom,"comment");
}

void printToken(TcurrentSymbole sym){
	if (currentSymbole.CODE != COMMENT_TOKEN) {
	printf("%d\n",currentSymbole.CODE);
	printf("%s\n",currentSymbole.nom);
	}
}

void openFile(char* fileName){
	file = fopen(fileName, "r");
}

int IgnoredCaracters(char currentCaractere){
	if(currentCaractere==' '||currentCaractere=='\t'||currentCaractere=='\n')
		return 1;
	return 0;
}


int AlphaNum(char currentCaractere){
	if((currentCaractere>='a' && currentCaractere<='z')||(currentCaractere>='A' && currentCaractere<='Z')||(currentCaractere>='0' && currentCaractere<='9') || (currentCaractere=='_') || (currentCaractere=='.'))
		return 1;
	return 0;
}

int CaractereSigne(char currentCaractere){
if(currentCaractere=='+'||currentCaractere=='*'||currentCaractere=='-'||currentCaractere=='<'||currentCaractere=='>'||currentCaractere=='/'||currentCaractere==';'||currentCaractere==')'
||currentCaractere=='}'||currentCaractere==','||currentCaractere==']'||currentCaractere=='^'||currentCaractere==':'||currentCaractere=='!')
		return 1;
	return 0;
}


int main(){
	openFile("testfile.r");
	if (file == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
	readCaractere();
	while(currentCaractere != EOF ){
		nextSymbole();
		printToken(currentSymbole);
	}
	//pour afficher la fin du fichier
	return 1;
}

