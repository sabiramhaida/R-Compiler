# minicompiler-project-R ll(3)
### contributers :
* [Sabir AMHAIDA](https://github.com/sabiramhaida/)
* [ANAM Aymane](https://github.com/anamaymane)
* [Mohammed ABDELOUHAB](https://github.com/Mohammed-Abdelouhab)
* [Hajar ATBIB](https://github.com/hajaratbib)

## Project structure
1. Lexical parser
2. syntaxic analyser
3. semantic analyser
## features:
* intermediate code generator
* ...

## 1. Lexical parser

# Grammaire :

	D = ("<-" | "=") , "function" , "(" , ["identifier",{(",","identifier")}] , ")", "{" , {STATEMENT} , [RETURN] , "}" 
		| C | "(" , ARGUMENTS , ")", [";"];

	ARGUMENTS = EXPRESSION , {"," , EXPRESSION};

	FUNCTIONCALL = "identifier" , "(" , ARGUMENTS , ")";

	EXPRESSION = "identifier" , E
		     | TYPE , B
		     | "!" , EXPRESSION , B;
	E = C | "(" , ARGUMENTS , ")";

	C = "[" , "number" , "]" , B | A , B;

	B = OPERATION , EXPRESSION , B | ;
	A = "$" , "identifier" | ("<-" | "=") , EXPRESSION | ;

	TYPE = VECTOR | LIST;

	VECTOR = "c" , "(" , ARGUMENTS , ")" | VALUE;

	LIST = "list" , "(" , "identifier" , "=" , TYPE , {"," , "identifier" , "=" , TYPE} , ")";

	VALUE = "number" | "string" | "boolean";

	OPERATION = "+" | "-" | "*" | "/" | ">" | "<" | ">=" | "<=" | "==" | "!=" | "|" | "&" | "^";

	STATEMENT = (LOOP | CONDITION | EXPRESSION | "identifier", D) , [";"] | RETURN;

	RETURN = "return" , "(" , EXPRESSION , ")", [";"];

	CONDITION = "if" , "(" , EXPRESSION , ")" , "{",STATEMENT ,"}", ["else" ,"{", STATEMENT,"}"];

	LOOP = FORLOOP | REPEATLOOP | WHILELOOP;

	FORLOOP = "for" , "(" , "identifier" , "in" , ("number" | FUNCTIONCALL | "identifier") ,
		":" , ("number" | FUNCTIONCALL | "identifier") , ")" , "{" ,{STATEMENT},"}";

	REPEATLOOP = "repeat" , "{" , {STATEMENT} , "}";

	WHILELOOP = "while" , "(" , EXPRESSION , ")" , "{" , {STATEMENT} , "}";
  
