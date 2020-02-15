// Changes Line Terminal depending on the environment (\r\n for Windows, \n for Linux)
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__WIN32__) || defined(__WIN32)
#define LT "\r\n"
#elif defined(__linux__)
#define LT "\n"
#endif

// Tokens enum
enum tokens {
    ID_TOKEN,
    STRING_TOKEN,
    BOOLEAN_TOKEN,
    EOF_TOKEN,
    FUNC_TOKEN,
    OP_TOKEN,
    CP_TOKEN,
    OCB_TOKEN,
    CCB_TOKEN,
    COMMA_TOKEN,
    OSB_TOKEN,
    CSB_TOKEN,
    EXCLAMATION_TOKEN,
    DOLLAR_TOKEN,
    C_TOKEN,
    LIST_TOKEN,
    NUMBER_TOKEN,
    DOT_TOKEN,
    UNDERSCORE_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN,
    MULT_TOKEN,
    DIV_TOKEN,
    GT_TOKEN,
    LT_TOKEN,
    GE_TOKEN,
    LE_TOKEN,
    EQ_TOKEN,
    DIFF_TOKEN,
    OR_TOKEN,
    AND_TOKEN,
    POW_TOKEN,
    AFF_TOKEN,
    RET_TOKEN,
    IF_TOKEN,
    ELSE_TOKEN,
    FOR_TOKEN,
    IN_TOKEN,
    COLON_TOKEN,
    REPEAT_TOKEN,
    WHILE_TOKEN,
    ERROR_TOKEN,
    SEMICOLON_TOKEN,
    COMMENT_TOKEN,
    PRINT_TOKEN
};

// Struct with token description (string) and its descriptor (int) in enum
typedef struct TOKEN_VALUE {
   char value[1024];
   int token;
} TOKEN_VALUE;

// See if we are in begin
int beginId = 0;
//Function argument counter
int functionArgumentCounter = 0;

// Array of tokens values and their descriptors
TOKEN_VALUE tabTokenvalue[43] = {
    {"valid R identifier",ID_TOKEN},
    {"string"            ,STRING_TOKEN},
    {"boolean"           ,BOOLEAN_TOKEN},
    {"eof"               ,EOF_TOKEN},
    {"function"         ,FUNC_TOKEN},
    {"("                 ,OP_TOKEN},
    {")"                 ,CP_TOKEN},
    {"{"                 ,OCB_TOKEN},
    {"}"                 ,CCB_TOKEN},
    {","                 ,COMMA_TOKEN},
    {"["                 ,OSB_TOKEN},
    {"]"                 ,CSB_TOKEN},
    {"!"                 ,EXCLAMATION_TOKEN},
    {"$"                 ,DOLLAR_TOKEN},
    {"c"                 ,C_TOKEN},
    {"list"              ,LIST_TOKEN},
    {"Number"            ,NUMBER_TOKEN},
    {"."                 ,DOT_TOKEN},
    {"_"                 ,UNDERSCORE_TOKEN},
    {"+"                 ,PLUS_TOKEN},
    {"-"                 ,MINUS_TOKEN},
    {"*"                 ,MULT_TOKEN},
    {"/"                 ,DIV_TOKEN},
    {">"                 ,GT_TOKEN},
    {"<"                 ,LT_TOKEN},
    {">="                ,GE_TOKEN},
    {"<="                ,LE_TOKEN},
    {"=="                ,EQ_TOKEN},
    {"!="                ,DIFF_TOKEN},
    {"|"                 ,OR_TOKEN},
    {"&"                 ,AND_TOKEN},
    {"^"                 ,POW_TOKEN},
    {"== or <-"          ,AFF_TOKEN},
    {"RETURN"            ,RET_TOKEN},
    {"if"                ,IF_TOKEN},
    {"else"              ,ELSE_TOKEN},
    {"for"               ,FOR_TOKEN},
    {"in"                ,IN_TOKEN},
    {":"                 ,COLON_TOKEN},
    {"repeat"            ,REPEAT_TOKEN},
    {"while"             ,WHILE_TOKEN},
    {"Error"             ,ERROR_TOKEN},
    {";"                 ,SEMICOLON_TOKEN}
};

//Global variables symbols table
typedef struct _TABLE_SYMBOLE{
  int token;
  char name[256];
  void * adresse;
  int size; // Number of bytes 
} _TABLE_SYMBOLE;

// A single node of _SCANNER_OUTPUT_LIST linked list
typedef struct _SCANNER_OUTPUT_NODE {
    char name[256];
    int token;
} _SCANNER_OUTPUT_NODE;

// Linked list containing token value/token descriptor pairs
typedef struct _SCANNER_OUTPUT_LIST{
   _SCANNER_OUTPUT_NODE node;
   struct _SCANNER_OUTPUT_LIST * next;
} _SCANNER_OUTPUT_LIST;

// File to be scanned (the output of the lexer)
char scannerOutputFile[1024] = "scannerOutputFile.txt";

// A single variable structure (symbols table single element)
typedef struct variable {
    char name[1024]; // id of variable
    int token;
    char class[1024];
    int length;               
    int maxElementInVariable;
    struct variables ** elements; //Null if it's an elementary type   //use calloc if we exceed max value
    int size; //number of bytes
    char value[1024];
    struct variable * next;
    struct variable * nextActifVariable;
} variable;


// Scanner functions, corresponds to Non-Terminals in grammar
void D();
void LOOP();
void LOP();
void LOGEXP();
void FUNCTIONCALL();
void CONDITION();
void TYPE();
void B();
void EXPRESSION();
void RETURN();
void STATEMENT();
void C();
void ARGUMENTS();
void E();
void A();
void OPERATION();
void VECTOR();
void LIST();
void VALUE();
void FORLOOP();
void WHILELOOP();
void REPEATLOOP();
