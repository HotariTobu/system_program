%{
#include <stdio.h>
#include <ctype.h>

int yylex();
int yyerror(char*);

int dbg = 0;
%}

%union {
    int ival;
}

%type <ival> expr
%type <ival> term
%type <ival> factor
%type <ival> digit

%%
statement: expr '=' { printf("\b%d\n", $1); /* $1: expr value */ };

expr: term { $$ = $1; }
    | expr '+' term { $$ = $1 + $3; /* $1: expr, $2: term */ }
    | expr '-' term { $$ = $1 - $3; /* $2: '-' */  };

term: factor { $$ = $1; }
    | term '*' factor { $$ = $1 * $3; }
    | term '/' factor { $$ = $1 / $3; };

factor: digit { $$ = $1; }
      | '(' expr ')' { $$ = $2; }
      | '-' factor { $$ = - $2 };

digit: '1' { $$ = 1; }
     | '2' { $$ = 2; }
     | '3' { $$ = 3; }
     | '4' { $$ = 4; }
     | '5' { $$ = 5; }
     | '6' { $$ = 6; }
     | '7' { $$ = 7; }
     | '8' { $$ = 8; }
     | '9' { $$ = 9; }
     | '0' { $$ = 0; };
%%

int yylex() {
    int ch = getchar();

    if (dbg > 0) {
        printf("ch=%d(%c) ", ch, ch);
    }

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
