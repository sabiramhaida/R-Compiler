
variable * currentVariable; // Current variable to be treated
variable * activeVariable;   // In case of a complex type (vector), points to the parent variable while treating its childs
variable * headVariable = NULL;  // Head of the the symbols table (linked list), initialised to NULL
int tableSymboleCounter = 0;
int vectorArgument = 0;
char argBefore[1024] = {"temp"};
char argAfter[1024]= {"temp"};
//  Used for indenting
static int Counter = 0;
// Indent function prints tabulations
void Indent(int number) {
    if (number > 0) {
        for (int i = 0; i < number; i++) {
            printf("\t");
        }
    }
}

// Points to the current token to be parsed
_SCANNER_OUTPUT_LIST * cursor;

// Creates head of tokens linked list
_SCANNER_OUTPUT_LIST * createHead(char * name, int token){
   _SCANNER_OUTPUT_LIST * list = (_SCANNER_OUTPUT_LIST *)malloc(sizeof(_SCANNER_OUTPUT_LIST));
   list->node.token = token;
   strcpy(list->node.name, name);
   list->next = NULL;
   cursor = list;
   return list;
}

// Adds a node to the tokens linked list
_SCANNER_OUTPUT_LIST * addNode(_SCANNER_OUTPUT_LIST * head, char * name, int token){
  _SCANNER_OUTPUT_LIST * cursor;
  _SCANNER_OUTPUT_LIST * list = (_SCANNER_OUTPUT_LIST *)malloc(sizeof(_SCANNER_OUTPUT_LIST));
  list->node.token = token;
  strcpy(list->node.name ,name);
  if(head->next == NULL) head->next = list;
  else {
      for(cursor = head ; cursor->next != NULL; cursor = cursor->next); cursor->next = list;
  }
  list->next = NULL;
  return head;
}

// Fills the list from an output file (from the lexer)
_SCANNER_OUTPUT_LIST * extractList() {
    int counter = 0;
    char temp[10240], tempBefore[10240];
    char *format_string = calloc(128, sizeof(char));
    sprintf(format_string, "%%[^%s]%s", LT, LT);
    _SCANNER_OUTPUT_LIST * list;
    FILE *f = fopen(scannerOutputFile, "r");
    if (f != NULL) {
        while (!feof(f)) {
            counter++;
            strcpy(tempBefore, temp);
            fscanf(f, format_string, temp);
            if (counter == 2 ) {
                list = createHead(temp, atoi(tempBefore));
            } else if (counter % 2 == 0) {
                list = addNode(list, temp, atoi(tempBefore));
            }
        }
    } else {
        char * error_msg = calloc(1024, sizeof(char));
        sprintf(error_msg, "File can't be opened%s", LT);
        perror(error_msg);
        exit(EXIT_FAILURE);
    }
    return list;

}

// Prints list of tokens
void showList(_SCANNER_OUTPUT_LIST * head){
    _SCANNER_OUTPUT_LIST * p1;
     for(p1 = head;  p1!= NULL; p1=p1->next){
        printf("%s ", p1->node.name);
     }
}

// Creates a variable to be added to the symbols table
variable * createVariable(char name[1024], int token, char * class,int length,int size){
    variable * v = malloc(sizeof(variable));
    strcpy(v->name,name);
    v->token = token;
    strcpy(v->class, class);
    strcpy(v->value, "NONE");
    v->length = length;
    v->size = size;
    v->maxElementInVariable = 1024;
    v->elements = malloc(v->maxElementInVariable * sizeof(variable *));
    v->next = NULL;
}

// Adds variable to another variable (makes it a child of the latter)
void addVariable(variable * sup, variable * currentVariable) {
   //Checking if the containing array isn't full
   if(sup->length == sup->maxElementInVariable) {
       variable ** elements = malloc(++sup->maxElementInVariable * sizeof(variable *));
       for(int i = 0; i < sup->maxElementInVariable - 2; i++) elements[i] = sup->elements[i];
       sup->elements = elements;
   }
   // Adding node to list
   currentVariable->maxElementInVariable = 1024;
   currentVariable->elements = malloc(currentVariable->maxElementInVariable * sizeof(variable *));
   sup->length++;
   sup->elements[sup->length - 1] = currentVariable;
   currentVariable->next = NULL;
}

// Adds variable to the symbols table
variable * addToVariableList(variable * headVariable,variable * v){
    variable *p = headVariable;
    if(strcmp(v->class,"FUNCTION ARGUMENT") != 0){
        if(headVariable == NULL){
        headVariable  = v;
        headVariable->next = NULL;
    }
    else {
        for(p = headVariable; p->next != NULL; p = p->next);
        p->next = v; 
        v->next = NULL;
    }
    }
    return headVariable;
}

// Adds variable to the active variables list
variable * addToActifVariableList(variable * actif, variable * v){
    if(actif == NULL){
        v->nextActifVariable = NULL;
    }
    else {
        v->nextActifVariable = actif;
    }
    return v;
}

// Removes variable to the active variables list
variable * removeFromActifVariableList(variable * actif){
    if(actif == NULL || actif->nextActifVariable == NULL) return NULL;
    return actif->nextActifVariable;
}

// Shows fields of specified variable
void showVariable(variable * v){ 
    int a = ++tableSymboleCounter; 
   Indent(a);
   printf("name=%s\n",v->name);
   Indent(a);
   printf("token=%d\n",v->token);
   Indent(a);
   printf("class=%s\n",v->class);
   Indent(a);
   printf("length=%d\n",v->length);
   Indent(a);
   printf("value=%s\n",v->value);   
   if(v->length > 0) for(int i = 0; i < v->length; i++) {
           Indent(a+1);
           printf("|_\n");
           showVariable(v->elements[i]);
           printf("\n");
           }
    else printf("\n");
    --tableSymboleCounter;
}

// Shows variables in list
void showVariableList(variable * head){
    printf("\t\t\t\t\t\t\t\t\t\tTable des symboles : \n\n");
    for(variable * p = head; p!= NULL; p = p->next){
        printf("Variable %s begin  --------------------------------------------\n\n",p->name);
        showVariable(p);
        printf("Variable %s end --------------------------------------------\n\n",p->name);
    }
}

// Seeks if a variable exists in list
int testIfaVariableExist(variable * head, char * name){
    for(variable * p = head; p!= NULL; p = p->next){
        if(strcmp(p->name,name) == 0) return 1;
    }
    return 0;
}

// Removes a variable from the list
variable * removeVariableFromList(variable * head,char * name){
    for(variable * p = head; p!= NULL; p = p->next){
        if(strcmp(head->name,name) == 0){
            if(head->next == NULL) return NULL;
            else head = head->next;
            p->next = NULL;
            free(p);
            return head;
            }
        else if(p->next != NULL){
            if(strcmp(p->next->name,name) == 0){
                p->next = p->next->next;
                p->next->next = NULL;
                free(p->next);
            }
        }
    }
}

// Returns the address of a variable in list
variable * findVariableInList(variable * head,char * name){
    variable *temp;
    for(variable * p = head; p!= NULL; p = p->next){
        if(strcmp(p->name,name) == 0){
            return p;
        }
        for(int i = 0; i < p->length; i++){
             temp = p->elements[i];
             if(strcmp(temp->name,name) == 0){
                return temp;
             }
        }
    }
}


// Tests if a variable is a child of the specified variable
int testIfaVariableExistInOtherVariable(variable * head, char * nameSup, char * nameChild ){
    variable * temp;
    for(variable * p = head; p!= NULL; p = p->next){
        if(strcmp(p->name,nameSup) == 0){
            for(int i = 0; i < p->length; i++){
                temp = p->elements[i];
                if(strcmp(temp->name, nameChild) == 0){
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Copies a variable to another
void copyToVariable(variable * dest, variable * source){
    dest->token = source->token;
    strcpy(dest->class, source->class);
    dest->length = source->length;
    dest->maxElementInVariable = source->maxElementInVariable;
    for(int i = 0 ; i < source->length; i++){
           dest->elements[i] = source->elements[i];
    }
    dest->size = source->size;
    strcpy(dest->value, source->value);
}

// Returns the description of a token
char * findValueOfToken(int token){
    for(int i = 0; i < 43; i++){
        if(token == tabTokenvalue[i].token){
            return tabTokenvalue[i].value;
        }
    }
}

// Tests if variables are of the same type
void testIfVariableOfTheSameType(char * name1,char * name2,char * class1, char * class2){
    char temp[2048];
    if(strcmp(class1,class2) != 0) {
        sprintf(temp,"Variable %s and variable %s aren't from the same class,found two different class : %s and %s",name1,name2,class1,class2);
        printf("%s%s",temp,LT);
        exit(EXIT_FAILURE);
    }
}

variable * testIfFunctionArgument(variable * head, char * name){
    variable * temp;
    for(variable * p = head; p != NULL; p = p->nextActifVariable){    
        //printf("testing if %s is valid as argument on function %s\n",p->name,p->class);
        if(strcmp(p->class, "FUNCTION") == 0){
            //printf("func : testing if %s is valid as argument on function %s\n",p->name,p->class);
            for(int i = 0; i< p->length; i++){
                temp = p->elements[i];
                if(strcmp(temp->name,name) == 0) {
                    return temp;
                }
            }
        }
    }
    return NULL;
}

// Seeks if a variable exists in list
int testIfaFunctionExist(variable * head, char * name){
    for(variable * p = head; p!= NULL; p = p->next){
        if(strcmp(p->name,name) == 0 && strcmp(p->class,"FUNCTION") == 0) return 1;
    }
    return 0;
}

