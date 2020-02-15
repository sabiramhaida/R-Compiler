gcc lexCompiler.c -w -o lex &
./lex
gcc parser.c -w -o parser
./parser
rm scannerOutputFile.txt