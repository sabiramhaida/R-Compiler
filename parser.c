#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.c"
#include "error.c"


// tests if the current token is the expected symbol (for terminals testing)
void Test_Symbole(int Token, char * stringErr){
    char message[1024];
    if(cursor == NULL) Erreur("Syntaxe fausse");
    if(cursor->next == NULL && cursor->node.token != EOF_TOKEN){
        Erreur("FINAL TOKEN is not EOF");
    } 
    else if(cursor->node.token == Token){
        sprintf(message,"<<\t\t<< TOKEN %d ACCEPTED, name is \"%s\"",Token,cursor->node.name);
        FILE * log = fopen("log.txt","a");
        fprintf(log,"%s\n",message);
        fclose(log);
        cursor = cursor->next;
    } else {
        printf("Expected \'%s\', found \'%s\'\n",findValueOfToken(Token),cursor->node.name);
        Erreur(stringErr);
    }
}

// no ambiguity, no errors
void BODY(){
    variable * temp;
    while(cursor->node.token != EOF_TOKEN){
        if(cursor->node.token == ID_TOKEN && cursor->next->node.token == OSB_TOKEN){ 
            currentVariable = createVariable(cursor->node.name, cursor->node.token, "NONE", 0, 2);
            Test_Symbole(ID_TOKEN,"BODY_ERR"); 
            D();
        }
        else {
          if(cursor->node.token == ID_TOKEN){
            if(testIfaVariableExist(headVariable,cursor->node.name) == 1) currentVariable = findVariableInList(headVariable,cursor->node.name);
            else currentVariable = createVariable(cursor->node.name, cursor->node.token, "NONE", 0, 2);
            //
            Test_Symbole(ID_TOKEN,"BODY_ERROR_1");
            if(cursor->node.token == AFF_TOKEN && cursor->next->node.token == ID_TOKEN && cursor->next->next->node.token == OP_TOKEN) {
                if(testIfaFunctionExist(headVariable,cursor->next->node.name) == 0) ErrorFunctionNotDeclared(cursor->next->node.name);
                activeVariable = addToActifVariableList(activeVariable , currentVariable);
                headVariable = addToVariableList(headVariable,currentVariable);
                Test_Symbole(AFF_TOKEN,"BODY_ERR");
                FUNCTIONCALL();
            }else {
                if(cursor->node.token == OP_TOKEN){
                    if(testIfaFunctionExist(headVariable,currentVariable->name) == 0) ErrorFunctionNotDeclared(currentVariable->name);
                    else activeVariable = addToActifVariableList(activeVariable , currentVariable);
                }
                else {
                    
                    activeVariable = addToActifVariableList(activeVariable , currentVariable);
                }
                D(); 
                if(strcmp(currentVariable->class,"NONE") != 0){
                    if(testIfaVariableExist(headVariable,currentVariable->name) == 1) {
                        temp = findVariableInList(headVariable,currentVariable->name);
                        copyToVariable(temp,currentVariable);
                    }
                    else if(strcmp(currentVariable->name,"$temp$") != 0 && strcmp(currentVariable->name,"NUMBER") != 0 && strcmp(currentVariable->name,"STRING") != 0 && strcmp(currentVariable->name,"BOOLEAN") != 0) 
                    headVariable = addToVariableList(headVariable,currentVariable);
                   
                }
                else if(testIfaVariableExist(headVariable,currentVariable->name) == 0 )ErrorVariableNotDeclared(currentVariable->name);  //IT COULD BE SOURCE OF PROBLEMS, same in statement
            }
        }
        else if(cursor->node.token == FOR_TOKEN || cursor->node.token == REPEAT_TOKEN || cursor->node.token == WHILE_TOKEN){
            LOOP();
        }
        else if(cursor->node.token == IF_TOKEN){
            CONDITION();
        }
        else if(cursor->node.token == C_TOKEN || cursor->node.token == NUMBER_TOKEN || cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN
        || cursor->node.token == LIST_TOKEN){
            TYPE();
            B();
        }
        else if(cursor->node.token == EXCLAMATION_TOKEN){
            Test_Symbole(EXCLAMATION_TOKEN,"BODY_ERROR_2");
            EXPRESSION();
            B();
        }
        else Erreur("BODY_ERROR");
        if(cursor->node.token == SEMICOLON_TOKEN) Test_Symbole(SEMICOLON_TOKEN,"BODY_ERROR_3");
        }
    }
}

// no ambiguity, no errors
void D(){
    if(cursor->node.token == AFF_TOKEN && cursor->next->node.token == FUNC_TOKEN){
        if(activeVariable != NULL && strcmp(activeVariable->class, "FUNCTION") == 0) {
            ErrorCantDeclareFunctionInsideFunction(activeVariable->name); 
        }
        activeVariable = addToActifVariableList(activeVariable , currentVariable);
        headVariable = addToVariableList(headVariable,currentVariable);
        strcpy(activeVariable->class,"FUNCTION");
        Test_Symbole(AFF_TOKEN,"D_ERROR_1");
        Test_Symbole(FUNC_TOKEN,"D_ERROR_2");
        Test_Symbole(OP_TOKEN,"D_ERROR_3");
        if(cursor->node.token != CP_TOKEN){ 
              currentVariable = createVariable(cursor->node.name, cursor->node.token, "FUNCTION ARGUMENT", 0, 2);
              Test_Symbole(ID_TOKEN,"D_ERROR_4");
              addVariable(activeVariable,currentVariable);
              while(cursor->node.token != CP_TOKEN) {
                  Test_Symbole(COMMA_TOKEN,"D_ERROR_5");
                  currentVariable = createVariable(cursor->node.name, cursor->node.token, "FUNCTION ARGUMENT", 0, 2);
                  Test_Symbole(ID_TOKEN,"D_ERROR_6");
                  addVariable(activeVariable,currentVariable);
              }
        }
        Test_Symbole(CP_TOKEN,"D_ERROR_7");
        Test_Symbole(OCB_TOKEN,"D_ERROR_8");
        if(cursor->node.token == RET_TOKEN){
            RETURN();
        }
        else 
        { 
          if(cursor->node.token == CCB_TOKEN){}
          else {
              while(cursor->node.token != CCB_TOKEN) STATEMENT();
          }
          if(cursor->node.token != CCB_TOKEN){
              RETURN();
          }
        }
        Test_Symbole(CCB_TOKEN,"D_ERROR_9");
        activeVariable = removeFromActifVariableList(activeVariable);
    }
    else if(cursor->node.token == AFF_TOKEN || cursor->node.token == OSB_TOKEN || cursor->node.token == PLUS_TOKEN || cursor->node.token == MINUS_TOKEN 
    || cursor->node.token == MULT_TOKEN || cursor->node.token == DIV_TOKEN || cursor->node.token == POW_TOKEN || cursor->node.token == DOLLAR_TOKEN
    || cursor->node.token == ID_TOKEN || cursor->node.token == CSB_TOKEN || cursor->node.token == CP_TOKEN || cursor->node.token == C_TOKEN 
    || cursor->node.token == NUMBER_TOKEN || cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN || cursor->node.token == LIST_TOKEN
    || cursor->node.token == COMMA_TOKEN || cursor->node.token == EXCLAMATION_TOKEN || cursor->node.token == FOR_TOKEN || cursor->node.token == REPEAT_TOKEN
    || cursor->node.token == CCB_TOKEN || cursor->node.token == WHILE_TOKEN || cursor->node.token == IF_TOKEN || cursor->node.token == ELSE_TOKEN
    || cursor->node.token == RET_TOKEN || cursor->node.token == EOF_TOKEN){
         C();
    }
    else if(cursor->node.token == OP_TOKEN){
        Test_Symbole(OP_TOKEN,"D_ERROR_7");
        ARGUMENTS();
        Test_Symbole(CP_TOKEN,"D_ERROR_8");
    }
    else Erreur("D_ERROR");
    if(cursor->node.token == SEMICOLON_TOKEN) Test_Symbole(SEMICOLON_TOKEN,"D_ERROR_9");
    activeVariable = NULL;
}

// no ambiguity, no errors
void ARGUMENTS(){
    EXPRESSION();
    functionArgumentCounter++;
    while(cursor->node.token == COMMA_TOKEN){
        Test_Symbole(COMMA_TOKEN,"ARGUMENTS_ERROR_1");
        EXPRESSION();
    functionArgumentCounter++;
    }
}

// no ambiguity, no errors
void FUNCTIONCALL(){
    ++Counter;
    char temp[1024];
    variable * tempVariable;
    if(findVariableInList(headVariable,cursor->node.name) == 0) ErrorFunctionNotDeclared(cursor->node.name);
    strcpy(temp,cursor->node.name);
    Test_Symbole(ID_TOKEN,"FUNCTIONCALL_ERROR_1");
    Test_Symbole(OP_TOKEN,"FUNCTIONCALL_ERROR_2");
    ARGUMENTS();
    tempVariable = findVariableInList(headVariable,temp);
    if(functionArgumentCounter != tempVariable->length) ErrorFunctionArgumentNumberNonValid(temp);
    functionArgumentCounter = 0;
    Test_Symbole(CP_TOKEN,"FUNCTIONCALL_ERROR_3");
}

// no ambiguity, no errors
void EXPRESSION(){
    char temp[1024]; //Used to choose which class between STRING and NUMBER
    if(cursor->node.token == ID_TOKEN ){
        currentVariable = createVariable(cursor->node.name, cursor->node.token, "NONE", 0, 2);
        if(cursor->next->node.token == OSB_TOKEN){}
        else {
        if(activeVariable != NULL){
            if(strcmp(activeVariable->class,"VECTOR") == 0) {
                if(cursor->node.token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
                else strcpy(temp, "STRING");
                addVariable(activeVariable,currentVariable);
            }
            else if(strcmp(activeVariable->class,"LIST") == 0){
                strcpy(currentVariable->class,"LIST MEMBER");
                if(cursor->node.token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
                else strcpy(temp, "STRING");
                addVariable(activeVariable,currentVariable);
            }
            else if(strcmp(activeVariable->class,"CLASS WILL RECIEVE") == 0){
                if(cursor->node.token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
                else strcpy(temp, "STRING");
                strcpy(activeVariable->class,temp);
                strcpy(activeVariable->value,cursor->node.name);
            }
        } else {
            if(cursor->node.token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
            else strcpy(temp, "STRING");
            currentVariable = createVariable(cursor->node.name,cursor->node.token,temp,0,2);
            headVariable = addToVariableList(headVariable,currentVariable);
        }
        Test_Symbole(ID_TOKEN,"EXPRESSION_ERROR_1");
        E();
        }
        
    }
    else if(cursor->node.token == C_TOKEN || cursor->node.token == NUMBER_TOKEN||cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN || cursor->node.token == LIST_TOKEN){
            TYPE();
            B();
    }
    else if(cursor->node.token == EXCLAMATION_TOKEN){
        Test_Symbole(EXCLAMATION_TOKEN,"EXPRESSION_ERROR_2");
        EXPRESSION();
        B();
    }
    else Erreur("EXPRESSION_ERROR");
}

// log expression
void LOGEXP(){
    char temp[1024];
    variable * tempVariable;
    if(cursor->node.token == OP_TOKEN){
        Test_Symbole(OP_TOKEN,"LOGERR");
        LOGEXP();
        Test_Symbole(CP_TOKEN,"LOGERR");
        if(cursor->node.token == AND_TOKEN) {
            Test_Symbole(AND_TOKEN,"LOGERR");
            LOGEXP();
        }
        else if(cursor->node.token == OR_TOKEN) {
            Test_Symbole(OR_TOKEN,"LOGERR");
            LOGEXP();
        }
    }
    else if(cursor->node.token == ID_TOKEN || cursor->node.token == NUMBER_TOKEN || cursor->node.token == BOOLEAN_TOKEN || cursor->node.token == STRING_TOKEN){
        if(cursor->node.token == ID_TOKEN) {
            tempVariable = testIfFunctionArgument(activeVariable, cursor->node.name);
            if(tempVariable == NULL){
                 if(testIfaVariableExist(headVariable,cursor->node.name) == 0) ErrorVariableNotDeclared(cursor->node.name);
                 currentVariable = findVariableInList(headVariable,cursor->node.name);
            }
            else currentVariable = tempVariable;
            activeVariable = addToActifVariableList(activeVariable , currentVariable);
            //if(testIfaVariableExist(headVariable, cursor->node.name) == 0) ErrorVariableNotDeclared(cursor->node.name);
            Test_Symbole(ID_TOKEN,"LOGERR");
        }
        else {
            if(cursor->node.token == NUMBER_TOKEN) strcpy(temp,"NUMBER");
            else if(cursor->node.token == BOOLEAN_TOKEN) strcpy(temp,"BOOLEAN");
            else strcpy(temp,"STRING");
            currentVariable = createVariable(cursor->node.name, cursor->node.token, temp, 0, 2);
            activeVariable = addToActifVariableList(activeVariable , currentVariable);
            VALUE();
            //activeVariable = removeFromActifVariableList(activeVariable);
        }
        LOP();
        if(cursor->node.token == ID_TOKEN) {
            tempVariable = testIfFunctionArgument(activeVariable, cursor->node.name);
            if(tempVariable == NULL){
                 if(testIfaVariableExist(headVariable,cursor->node.name) == 0) ErrorVariableNotDeclared(cursor->node.name);
                 currentVariable = findVariableInList(headVariable,cursor->node.name);
            }
            else currentVariable = tempVariable;
            tempVariable = findVariableInList(headVariable,cursor->node.name);
            testIfVariableOfTheSameType(activeVariable->name,cursor->node.name,activeVariable->class,tempVariable->class);
            Test_Symbole(ID_TOKEN,"LOGERR");
        }
        else {
            if(cursor->node.token == NUMBER_TOKEN) strcpy(temp,"NUMBER");
            else if(cursor->node.token == BOOLEAN_TOKEN) strcpy(temp,"BOOLEAN");
            else strcpy(temp,"STRING");
            currentVariable = createVariable(cursor->node.name, cursor->node.token, temp, 0, 2);
            if(strcmp("FUNCTION ARGUMENT", currentVariable->class) == 0){
                testIfVariableOfTheSameType(activeVariable->name,cursor->node.name,activeVariable->class,currentVariable->class);
            }
            VALUE();
            activeVariable = removeFromActifVariableList(activeVariable);
        }
        //activeVariable = removeFromActifVariableList(activeVariable);
    }
    else if(cursor->node.token == BOOLEAN_TOKEN){
    }
}

// ambiguity, resorts to default cases, no errors, LL(3)
void E(){
    if(cursor->node.token == OP_TOKEN){
        Test_Symbole(OP_TOKEN,"E_ERROR_1");
        ARGUMENTS();
        Test_Symbole(CP_TOKEN,"E_ERROR_2");
    }
    else if(cursor->node.token == AFF_TOKEN || cursor->node.token == OSB_TOKEN || cursor->node.token == PLUS_TOKEN || cursor->node.token == MINUS_TOKEN
    || cursor->node.token == MULT_TOKEN || cursor->node.token == DIV_TOKEN || cursor->node.token == POW_TOKEN || cursor->node.token == DOLLAR_TOKEN
    || cursor->node.token == ID_TOKEN || cursor->node.token == CSB_TOKEN || cursor->node.token == CP_TOKEN || cursor->node.token == C_TOKEN 
    || cursor->node.token == NUMBER_TOKEN || cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN || cursor->node.token == LIST_TOKEN
    || cursor->node.token == COMMA_TOKEN || cursor->node.token == EXCLAMATION_TOKEN || cursor->node.token == FOR_TOKEN || cursor->node.token == REPEAT_TOKEN
    || cursor->node.token == CCB_TOKEN || cursor->node.token == WHILE_TOKEN || cursor->node.token == IF_TOKEN || cursor->node.token == ELSE_TOKEN
    || cursor->node.token == RET_TOKEN || cursor->node.token == EOF_TOKEN){
        C();
    }
    else if(cursor->node.token == OP_TOKEN && cursor->next != NULL){
        if(cursor->next->node.token == CSB_TOKEN || cursor->next->node.token == PLUS_TOKEN
            || cursor->next->node.token == MINUS_TOKEN || cursor->next->node.token == MULT_TOKEN || cursor->next->node.token == DIV_TOKEN 
            || cursor->next->node.token == POW_TOKEN
            || cursor->next->node.token == CP_TOKEN || cursor->next->node.token == COMMA_TOKEN || cursor->next->node.token == FOR_TOKEN || cursor->next->node.token == REPEAT_TOKEN
            || cursor->next->node.token == CCB_TOKEN || cursor->next->node.token == WHILE_TOKEN || cursor->next->node.token == IF_TOKEN || cursor->next->node.token == ELSE_TOKEN
            || cursor->next->node.token == RET_TOKEN || cursor->next->node.token == EOF_TOKEN || ((cursor->next->node.token == ID_TOKEN || cursor->next->node.token == NUMBER_TOKEN || cursor->next->node.token == C_TOKEN
            || cursor->next->node.token == STRING_TOKEN || cursor->next->node.token == BOOLEAN_TOKEN || cursor->next->node.token == LIST_TOKEN || cursor->next->node.token == EXCLAMATION_TOKEN) && (cursor->next->next->node.token != EXCLAMATION_TOKEN || cursor->next->next->node.token != C_TOKEN || 
            cursor->next->next->node.token != LIST_TOKEN || cursor->next->next->node.token != ID_TOKEN))) {
          C();
        }
    }
    else if(cursor->node.token == OP_TOKEN && cursor->next != NULL){
        if((cursor->next->node.token == ID_TOKEN || cursor->next->node.token == NUMBER_TOKEN || cursor->next->node.token == C_TOKEN
          || cursor->next->node.token == STRING_TOKEN || cursor->next->node.token == BOOLEAN_TOKEN || cursor->next->node.token == LIST_TOKEN || cursor->next->node.token == EXCLAMATION_TOKEN)
          && (cursor->next->next->node.token == EXCLAMATION_TOKEN || cursor->next->next->node.token == C_TOKEN || 
          cursor->next->next->node.token == LIST_TOKEN || cursor->next->next->node.token == ID_TOKEN)){
            Test_Symbole(OP_TOKEN,"E_ERROR_1");
            ARGUMENTS();
            Test_Symbole(CP_TOKEN,"E_ERROR_2");
        }
    }
    else Erreur("E_ERROR");
}

// no ambiguity, no errors
void C(){
    int index;
    if(cursor->node.token == OSB_TOKEN){    
        if(testIfaVariableExist(headVariable, currentVariable->name) == 0) ErrorVariableNotDeclared(currentVariable->name);
        Test_Symbole(OSB_TOKEN,"C_ERROR_1");
        if(cursor->node.token == NUMBER_TOKEN) {
            index = atof(cursor->node.name);
            currentVariable = findVariableInList(headVariable, currentVariable->name);
            if(!(currentVariable->length >= index))  ErrorIndexOutOfBoundOnVariable(currentVariable->name,cursor->node.name);
        }
        Test_Symbole(NUMBER_TOKEN,"C_ERROR");
        Test_Symbole(CSB_TOKEN,"C_ERROR_2");
        B();
    }
    else if(cursor->node.token == ID_TOKEN || cursor->node.token == AFF_TOKEN || cursor->node.token == CSB_TOKEN || cursor->node.token == PLUS_TOKEN 
    || cursor->node.token == MINUS_TOKEN || cursor->node.token == MULT_TOKEN || cursor->node.token == DIV_TOKEN || cursor->node.token == POW_TOKEN
    || cursor->node.token == DOLLAR_TOKEN || cursor->node.token == CP_TOKEN || cursor->node.token == C_TOKEN ||cursor->node.token == NUMBER_TOKEN 
    || cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN || cursor->node.token == LIST_TOKEN || cursor->node.token == COMMA_TOKEN
    ||cursor->node.token == EXCLAMATION_TOKEN || cursor->node.token == FOR_TOKEN || cursor->node.token == REPEAT_TOKEN || cursor->node.token == CCB_TOKEN
    || cursor->node.token == WHILE_TOKEN || cursor->node.token == IF_TOKEN || cursor->node.token == ELSE_TOKEN || cursor->node.token == RET_TOKEN
    || cursor->node.token == EOF_TOKEN)
    {
        A();
        B();
    }
    else Erreur("C_ERROR");
}

// ambiguity, no errors
void B(){
    if(cursor->node.token == PLUS_TOKEN || cursor->node.token == MINUS_TOKEN || cursor->node.token == MULT_TOKEN || cursor->node.token == DIV_TOKEN
    || cursor->node.token == POW_TOKEN){
        OPERATION();
        EXPRESSION(); 
        B();
    }
    if(cursor->node.token == ID_TOKEN || cursor->node.token == CSB_TOKEN || cursor->node.token == PLUS_TOKEN || cursor->node.token == MINUS_TOKEN
    || cursor->node.token == MULT_TOKEN || cursor->node.token == DIV_TOKEN || cursor->node.token == POW_TOKEN || cursor->node.token == CP_TOKEN
    || cursor->node.token == C_TOKEN || cursor->node.token == NUMBER_TOKEN || cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN
    || cursor->node.token == LIST_TOKEN || cursor->node.token == COMMA_TOKEN || cursor->node.token == EXCLAMATION_TOKEN || cursor->node.token == FOR_TOKEN
    || cursor->node.token == REPEAT_TOKEN || cursor->node.token == CCB_TOKEN || cursor->node.token == WHILE_TOKEN || cursor->node.token == IF_TOKEN
    || cursor->node.token == ELSE_TOKEN || cursor->node.token == RET_TOKEN || cursor->node.token == EOF_TOKEN || cursor->node.token == SEMICOLON_TOKEN){
    
    }
}

// no ambiguity, no errors
void A(){
    if(cursor->node.token == DOLLAR_TOKEN){
        if(testIfaVariableExist(headVariable, currentVariable->name) == 0) ErrorVariableNotDeclared(currentVariable->name);
        Test_Symbole(DOLLAR_TOKEN,"A_ERROR_1");
        if(cursor->node.token == ID_TOKEN) {
            if(testIfaVariableExistInOtherVariable(headVariable,currentVariable->name,cursor->node.name) == 0){
                ErrorNonValidArgumentForVariable(currentVariable->name,cursor->node.name);
            }
        }
        Test_Symbole(ID_TOKEN,"A_ERROR_2");
    }
    else if(cursor->node.token == AFF_TOKEN){
        strcpy(currentVariable->class,"CLASS WILL RECIEVE");
        activeVariable = addToActifVariableList(activeVariable,currentVariable);
        Test_Symbole(AFF_TOKEN,"A_ERROR_3");
        EXPRESSION();
        headVariable = addToVariableList(headVariable,activeVariable);  //A verifier
        activeVariable = removeFromActifVariableList(activeVariable);
    }
    else if(cursor->node.token == ID_TOKEN || cursor->node.token == CSB_TOKEN || cursor->node.token == PLUS_TOKEN || cursor->node.token == MINUS_TOKEN
    || cursor->node.token == MULT_TOKEN || cursor->node.token == DIV_TOKEN || cursor->node.token == POW_TOKEN || cursor->node.token == CP_TOKEN
    || cursor->node.token == C_TOKEN || cursor->node.token == NUMBER_TOKEN || cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN
    || cursor->node.token == LIST_TOKEN || cursor->node.token == COMMA_TOKEN || cursor->node.token == EXCLAMATION_TOKEN || cursor->node.token == FOR_TOKEN
    || cursor->node.token == REPEAT_TOKEN || cursor->node.token == CCB_TOKEN || cursor->node.token == WHILE_TOKEN || cursor->node.token == IF_TOKEN
    || cursor->node.token == ELSE_TOKEN || cursor->node.token == RET_TOKEN || cursor->node.token == EOF_TOKEN){
        //There is epsilon we should continue
    }
    else Erreur("A_ERROR");
}

// no ambiguity, no errors
void TYPE(){
    if(cursor->node.token == C_TOKEN || cursor->node.token == NUMBER_TOKEN || cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN){
        VECTOR();
    }
    else if(cursor->node.token == LIST_TOKEN) {
        LIST();
    }
    else Erreur("TYPE_ERROR");
}

// no ambiguity, no errors
void VECTOR(){
    char temp[1024];
    vectorArgument = 1;
    if(cursor->node.token == C_TOKEN){
        //Setting the activeVariable up
        activeVariable = addToActifVariableList(activeVariable , currentVariable);
        strcpy(activeVariable->class,"VECTOR");
        Test_Symbole(C_TOKEN,"VECTOR_ERROR_1");
        Test_Symbole(OP_TOKEN,"VECTOR_ERROR_2");
        ARGUMENTS();
        Test_Symbole(CP_TOKEN,"VECTOR_ERROR_3");
        headVariable = addToVariableList(headVariable,activeVariable);
        activeVariable = removeFromActifVariableList(activeVariable);
    }
    else if(cursor->node.token == NUMBER_TOKEN || cursor->node.token == STRING_TOKEN || cursor->node.token == BOOLEAN_TOKEN){
        VALUE();
    }
    else Erreur("VECTOR_ERROR");
    vectorArgument = 0;
}

// no ambiguity, no errors
void LIST(){
    variable * tempVariable;
    char temp[1024];
    if(cursor->node.token == LIST_TOKEN){
        //Setting the activeVariable up
        //activeVariable = addToActifVariableList(activeVariable , currentVariable);
        headVariable = addToVariableList(headVariable,activeVariable);
        tempVariable = activeVariable;
        strcpy(activeVariable->class,"LIST");
        //
        Test_Symbole(LIST_TOKEN,"LIST_ERROR_1");
        Test_Symbole(OP_TOKEN,"LIST_ERROR_2");
        currentVariable = createVariable(cursor->node.name, cursor->node.token, "NONE", 0, 2);
        Test_Symbole(ID_TOKEN,"LIST_ERROR_3");
                if(currentVariable->token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
                else strcpy(temp, "STRING");
                addVariable(activeVariable,currentVariable);
        strcpy(currentVariable->class,"CLASS WILL RECIEVE");
        activeVariable = addToActifVariableList(activeVariable,currentVariable);
        Test_Symbole(AFF_TOKEN,"LIST_ERROR_4");
        if(cursor->node.token == ID_TOKEN){
            if(testIfaVariableExist(headVariable,cursor->node.name) == 0){
                ErrorVariableNotDeclared(cursor->node.name);
            }
            else {
                currentVariable = createVariable(cursor->node.name, cursor->node.token, "NONE", 0, 2);
                strcpy(currentVariable->class,"LIST MEMBER");
                if(cursor->node.token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
                else if(cursor->node.token == STRING_TOKEN) strcpy(temp, "STRING");
                else if(cursor->node.token == BOOLEAN_TOKEN) strcpy(temp, "BOOLEAN");
                else {
                    copyToVariable(activeVariable,findVariableInList(headVariable,cursor->node.name));
                }
            }
            Test_Symbole(ID_TOKEN,"LIST_ERROR_*");
        }
        else TYPE();
        while(cursor->node.token == COMMA_TOKEN){
            activeVariable = removeFromActifVariableList(activeVariable);
            Test_Symbole(COMMA_TOKEN,"LIST_ERROR_5");
            currentVariable = createVariable(cursor->node.name, cursor->node.token, "NONE", 0, 2);
            Test_Symbole(ID_TOKEN,"LIST_ERROR_6");
                if(currentVariable->token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
                else strcpy(temp, "STRING");
                addVariable(activeVariable,currentVariable);
            strcpy(currentVariable->class,"CLASS WILL RECIEVE");
            activeVariable = addToActifVariableList(activeVariable,currentVariable);
            Test_Symbole(AFF_TOKEN,"LIST_ERROR_7");
           if(cursor->node.token == ID_TOKEN){
            if(testIfaVariableExist(headVariable,cursor->node.name) == 0){
                ErrorVariableNotDeclared(cursor->node.name);
            }
            else {
                currentVariable = createVariable(cursor->node.name, cursor->node.token, "NONE", 0, 2);
                strcpy(currentVariable->class,"LIST MEMBER");
                if(cursor->node.token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
                else if(cursor->node.token == STRING_TOKEN) strcpy(temp, "STRING");
                else if(cursor->node.token == BOOLEAN_TOKEN) strcpy(temp, "BOOLEAN");
                else {
                    copyToVariable(activeVariable,findVariableInList(headVariable,cursor->node.name));
                }
            }
            Test_Symbole(ID_TOKEN,"LIST_ERROR_*");
            
        }
        else TYPE();
        }
        Test_Symbole(CP_TOKEN,"LIST_ERROR_8");
        //Setting the activeVariable down
        activeVariable = removeFromActifVariableList(activeVariable);
    }
    else Erreur("LIST_ERROR");
    currentVariable = tempVariable;
}

// no ambiguity, no errors
void VALUE(){
    char temp[1024];
    if(strcmp(activeVariable->class,"CLASS WILL RECIEVE") == 0){
            if(cursor->node.token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
            else if(cursor->node.token == STRING_TOKEN) strcpy(temp, "STRING");
            else if(cursor->node.token == BOOLEAN_TOKEN) strcpy(temp, "BOOLEAN");
            strcpy(activeVariable->class,temp);
            strcpy(activeVariable->value,cursor->node.name);
    }
    else if(strcmp(activeVariable->class,"VECTOR") == 0 || strcmp(activeVariable->class,"LIST") == 0) {
                
                if(cursor->node.token == NUMBER_TOKEN) strcpy(temp, "NUMBER");
                else if(cursor->node.token == STRING_TOKEN) strcpy(temp, "STRING");
                else strcpy(temp, "BOOLEAN");
                strcpy(argBefore,temp);
                currentVariable = createVariable("$temp$", cursor->node.token, temp, 0, 2);
                strcpy(currentVariable->value,cursor->node.name);
                if(vectorArgument == 1 && functionArgumentCounter > 0 && strcmp(argBefore,argAfter) != 0 ) ErrorFunctionVectorArgumentOfDifferentClass();
                strcpy(argAfter,argBefore);
                addVariable(activeVariable,currentVariable);
            }
    if(cursor->node.token == NUMBER_TOKEN){
        Test_Symbole(NUMBER_TOKEN,"VALUE_ERROR_1");
    }
    else if(cursor->node.token == STRING_TOKEN){
        Test_Symbole(STRING_TOKEN,"VALUE_ERROR_2");
    }
    else if(cursor->node.token == BOOLEAN_TOKEN){
        Test_Symbole(BOOLEAN_TOKEN,"VALUE_ERROR_2"); 
    }
    else Erreur("VALUE_ERROR");
}

// no ambiguity, no errors
void OPERATION(){
    if(cursor->node.token == PLUS_TOKEN){
        Test_Symbole(PLUS_TOKEN,"OPERATION_ERROR_1");
    }
    else if(cursor->node.token == MINUS_TOKEN){
        Test_Symbole(MINUS_TOKEN,"OPERATION_ERROR_2");
    }
    else if(cursor->node.token == MULT_TOKEN){
        Test_Symbole(MULT_TOKEN,"OPERATION_ERROR_3");
    }
    else if(cursor->node.token == DIV_TOKEN){
        Test_Symbole(DIV_TOKEN,"OPERATION_ERROR_4");
    }
    else if(cursor->node.token == POW_TOKEN){
        Test_Symbole(POW_TOKEN,"OPERATION_ERROR_5");
    } 
    else Erreur("OPERATION_ERROR");
}

// logical operator
void LOP(){
    if(cursor->node.token == GT_TOKEN){
        Test_Symbole(GT_TOKEN,"OPERATION_ERROR_1");
    }
    else if(cursor->node.token == LT_TOKEN){
        Test_Symbole(LT_TOKEN,"OPERATION_ERROR_2");
    }
    else if(cursor->node.token == GE_TOKEN){
        Test_Symbole(GE_TOKEN,"OPERATION_ERROR_3");
    }
    else if(cursor->node.token == LE_TOKEN){
        Test_Symbole(LE_TOKEN,"OPERATION_ERROR_4");
    }
    else if(cursor->node.token == EQ_TOKEN){
        Test_Symbole(EQ_TOKEN,"OPERATION_ERROR_5");
    }
    else if(cursor->node.token == DIFF_TOKEN){
        Test_Symbole(DIFF_TOKEN,"OPERATION_ERROR_6");
    }
}

// no ambiguity, no errors
void STATEMENT(){
    variable * temp;
    if(cursor->node.token == FOR_TOKEN || cursor->node.token == REPEAT_TOKEN || cursor->node.token == WHILE_TOKEN){
        LOOP();
        if(cursor->node.token == SEMICOLON_TOKEN) Test_Symbole(SEMICOLON_TOKEN,"STATEMENT_ERROR_1");
    }
    else if(cursor->node.token == IF_TOKEN){
        CONDITION();
        if(cursor->node.token == SEMICOLON_TOKEN) Test_Symbole(SEMICOLON_TOKEN,"STATEMENT_ERROR_1");
    }
    
    else if(cursor->node.token == ID_TOKEN && cursor->next->node.token == AFF_TOKEN && cursor->next->next->node.token == FUNC_TOKEN){
        //Creating variable and adding it to list 
            currentVariable = createVariable(cursor->node.name, cursor->node.token, "NONE", 0, 2);
            
            //
        if(cursor->next->node.token == AFF_TOKEN) {
            Test_Symbole(ID_TOKEN,"STATEMENT_ERROR_1");
            D();
            if(strcmp(currentVariable->class,"NONE") != 0) {
                if(testIfaVariableExist(headVariable,currentVariable->name) == 1) {
                        temp = findVariableInList(headVariable,currentVariable->name);
                        copyToVariable(temp,currentVariable);
                    }
                    else if(strcmp(currentVariable->name,"$temp$") != 0) headVariable = addToVariableList(headVariable,currentVariable);
            }
            else if(testIfaVariableExist(headVariable,currentVariable->name) == 0 )ErrorVariableNotDeclared(currentVariable->name);
        }
        else {
            headVariable = addToVariableList(headVariable,currentVariable);
            EXPRESSION();
        }
        if(cursor->node.token == SEMICOLON_TOKEN) Test_Symbole(SEMICOLON_TOKEN,"STATEMENT_ERROR_1");
    }
    else if(cursor->node.token == ID_TOKEN && cursor->next->node.token == AFF_TOKEN && cursor->next->next->node.token == ID_TOKEN && cursor->next->next->next->node.token == OP_TOKEN) {
                if(testIfaFunctionExist(headVariable,cursor->next->next->node.name) == 0) ErrorFunctionNotDeclared(cursor->next->next->node.name);
                activeVariable = addToActifVariableList(activeVariable , currentVariable);
                headVariable = addToVariableList(headVariable,currentVariable);
                Test_Symbole(AFF_TOKEN,"BODY_ERR");
                FUNCTIONCALL();
            }
        else if(cursor->node.token == ID_TOKEN && cursor->next->node.token == AFF_TOKEN && cursor->next->next->node.token == ID_TOKEN) {
                if(testIfaVariableExist(headVariable,cursor->next->next->node.name) == 0) ErrorVariableNotDeclared(cursor->next->next->node.name);
                activeVariable = addToActifVariableList(activeVariable , currentVariable);
                headVariable = addToVariableList(headVariable,currentVariable);
                Test_Symbole(AFF_TOKEN,"BODY_ERR");
                FUNCTIONCALL();
            }
    else if(cursor->node.token == ID_TOKEN || cursor->node.token == C_TOKEN || cursor->node.token == NUMBER_TOKEN || cursor->node.token == STRING_TOKEN
    || cursor->node.token == BOOLEAN_TOKEN || cursor->node.token == LIST_TOKEN || cursor->node.token == EXCLAMATION_TOKEN){
        EXPRESSION();
        if(cursor->node.token == SEMICOLON_TOKEN) Test_Symbole(SEMICOLON_TOKEN,"STATEMENT_ERROR_1");
    }
    else if(cursor->node.token == RET_TOKEN) RETURN();
    else Erreur("STATEMENT_ERROR");
    if(cursor->node.token == SEMICOLON_TOKEN) Test_Symbole(SEMICOLON_TOKEN,"STATEMENT_ERROR_1");
}

// no ambiguity, no errors
void RETURN(){
    if(cursor->node.token == RET_TOKEN){
        Test_Symbole(RET_TOKEN,"RETURN_ERROR_1");  
        Test_Symbole(OP_TOKEN,"RETURN_ERROR_2");
        EXPRESSION();
        Test_Symbole(CP_TOKEN,"RETURN_ERROR_3");
    }
    else Erreur("RETURN_ERROR");
    if(cursor->node.token == SEMICOLON_TOKEN) Test_Symbole(SEMICOLON_TOKEN,"RETURN_ERROR_4");
}

// no ambiguity, no errors
void CONDITION(){
    if(cursor->node.token == IF_TOKEN){
        Test_Symbole(IF_TOKEN,"CONDITION_ERROR_1");
        Test_Symbole(OP_TOKEN,"CONDITION_ERROR_2");
        LOGEXP();
        Test_Symbole(CP_TOKEN,"CONDITION_ERROR_3");
        Test_Symbole(OCB_TOKEN,"CONDITION_ERROR_4");
        if(cursor->node.token == CCB_TOKEN){}
        else {
            while(cursor->node.token != CCB_TOKEN) STATEMENT();
        }
        if(cursor->node.token == CCB_TOKEN){
            Test_Symbole(CCB_TOKEN,"CONDITION_ERROR_5");
            if(cursor->node.token == ELSE_TOKEN){
                Test_Symbole(ELSE_TOKEN,"CONDITION_ERROR_6");
                Test_Symbole(OCB_TOKEN,"CONDITION_ERROR_6");
                if(cursor->node.token == CCB_TOKEN){}
                else {
                    while(cursor->node.token != CCB_TOKEN) STATEMENT();
                }
                Test_Symbole(CCB_TOKEN,"CONDITION_ERROR_6");
            }
        }
    }
    else Erreur("CONDITION_ERROR");
}

// no ambiguity, no errors
void LOOP(){
    if(cursor->node.token == FOR_TOKEN){
        FORLOOP();
    }
    else if(cursor->node.token == REPEAT_TOKEN){
        REPEATLOOP();
    }
    else if(cursor->node.token == WHILE_TOKEN){
        WHILELOOP();
    }
    else Erreur("LOOP_ERROR");
}

// no ambiguity, no errors
void FORLOOP(){
    if(cursor->node.token == FOR_TOKEN){
        Test_Symbole(FOR_TOKEN,"FORLOOP_ERROR_1");
        Test_Symbole(OP_TOKEN,"FORLOOP_ERROR_2");
        Test_Symbole(ID_TOKEN,"FORLOOP_ERROR_3");
        Test_Symbole(IN_TOKEN,"FORLOOP_ERROR_4");

        if(cursor->node.token == NUMBER_TOKEN) Test_Symbole(NUMBER_TOKEN,"");
        Test_Symbole(COLON_TOKEN,"FORLOOP_ERROR_6");

        if(cursor->node.token == NUMBER_TOKEN) Test_Symbole(NUMBER_TOKEN,"FORLOOP_ERROR_7");

        Test_Symbole(CP_TOKEN,"FORLOOP_ERROR_9");
        Test_Symbole(OCB_TOKEN,"FORLOOP_ERROR_10");
        if(cursor->node.token == CCB_TOKEN){}
        else {
            while(cursor->node.token != CCB_TOKEN) STATEMENT();
        }
        Test_Symbole(CCB_TOKEN,"FORLOOP_ERROR_11");
    }
    else Erreur("FORLOOP_ERROR");
}

// no ambiguity, no errors
void REPEATLOOP(){
    if(cursor->node.token == REPEAT_TOKEN){
        Test_Symbole(REPEAT_TOKEN,"REPEATLOOP_ERROR_1");
        Test_Symbole(OCB_TOKEN,"REPEATLOOP_ERROR_2");
        if(cursor->node.token == CCB_TOKEN){}
        else {
            while(cursor->node.token != CCB_TOKEN) STATEMENT();
        }
        Test_Symbole(CCB_TOKEN,"REPEATLOOP_ERROR_3");
    }
    else Erreur("REPEATLOOP_ERROR");
}

// no ambiguity, no errors
void WHILELOOP(){
    if(cursor->node.token == WHILE_TOKEN){
        Test_Symbole(WHILE_TOKEN,"WHILELOOP_ERROR_1");
        Test_Symbole(OP_TOKEN,"WHILELOOP_ERROR_2");
        LOGEXP();
        Test_Symbole(CP_TOKEN,"WHILELOOP_ERROR_3");
        Test_Symbole(OCB_TOKEN,"WHILELOOP_ERROR_4");
        if(cursor->node.token == CCB_TOKEN){}
        else {
            while(cursor->node.token != CCB_TOKEN) STATEMENT();
        }
        Test_Symbole(CCB_TOKEN,"WHILELOOP_ERROR_5");
    }
    else Erreur("WHILELOOP_ERROR");
}



int main(){
    _SCANNER_OUTPUT_LIST * list;
    remove("log.txt");
    printf("\t\t\t\t\t\t\t\t\t\t------------------Code saisie :-----------------\n\n");
    list = extractList();
    printf("%s", LT);
    showList(list);
    printf("\n\n\t\t\t\t\t\t\t\t\t\t----------------------------------------------\n\n");
    printf("\n\n********* Begin analysis *********%s\n", LT);
    BODY();
    printf("*********  End analysis  *********%s\n", LT);
    showVariableList(headVariable);
}
