// Variable doesn't exist in symbols table
void ErrorVariableNotDeclared(char * variableName){
    char temp[2048];
    sprintf(temp,"Variable %s not declared",variableName);
    printf("%s%s",temp,LT);
    exit(EXIT_FAILURE);
}

// Invalid argument for variable
void ErrorNonValidArgumentForVariable(char * sup, char * arg){
    char temp[2048];
    sprintf(temp,"Variable %s doesn't have an element called %s",sup,arg);
    printf("%s%s",temp,LT);
    exit(EXIT_FAILURE);
}

// Index out of bound for complex types
void ErrorIndexOutOfBoundOnVariable(char * sup, char * num){
    char temp[2048];
    sprintf(temp,"Index out on bound error, trying to access %s on variable %s",sup,num);
    printf("%s%s",temp,LT);
    exit(EXIT_FAILURE);
}

// Error when attempts to construct a vector with heterogeneous types
void ErrorDifferentArgumentTypeForVector(char * vectorName, char * argName){
    char temp[2048];
    sprintf(temp,"all VECTOR %s elements must be of the same type, problem occured on %s",vectorName,argName);
    printf("%s%s",temp,LT);
    exit(EXIT_FAILURE);
}

// Can't declare function inside another function
void ErrorCantDeclareFunctionInsideFunction(char * functionName){
    char temp[2048];
    sprintf(temp,"You can't declare function inside function, problem on function %s declaration",functionName);
    printf("%s%s",temp,LT);
    exit(EXIT_FAILURE);
}

// Calling a function that's not declared
void ErrorFunctionNotDeclared(char * name){
    char temp[2048];
    sprintf(temp,"Function %s wasn't declared before",name);
    printf("%s%s",temp,LT);
    exit(EXIT_FAILURE);
}

// Generic error type
void Erreur(char * name){
    FILE * log = fopen("log.txt","a");
    fprintf(log,"%s\n",name);
    fclose(log);
    exit(EXIT_FAILURE);
}

void ErrorFunctionArgumentNumberNonValid(char * name){
    char temp[2048];
    sprintf(temp,"error argument number error on function %s\n",name);
    printf("%s%s",temp,LT);
    exit(EXIT_FAILURE);
} 

void ErrorFunctionVectorArgumentOfDifferentClass(){
    char temp[2048];
    sprintf(temp,"Error : Vector must have elements of the same type\n");
    printf("%s%s",temp,LT);
    exit(EXIT_FAILURE);
} 