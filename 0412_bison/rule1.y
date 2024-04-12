%{
#include <stdio.h>
#include <ctype.h>

int yylex();
int yyerror(char*);
%}

%%
statement: expr '=';
expr: term | expr '+' term | expr '-' term;
term: factor | term '*' factor | term '/' factor;
factor: digit | '(' expr ')' | '-' factor;
digit: '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '0';
%%

int yylex() {
    int ch = getchar();
    if (ch == '\n' || ch == EOF) {
        return 0;
    }
    return ch;
}

int yyerror(char* m) {
    fprintf(stderr, "E: %s\n", m);
    return 0;
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
