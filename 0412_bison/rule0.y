%%
statement: expr '=';
expr: term | expr '+' term | expr '-' term;
term: factor | term '*' factor | term '/' factor;
factor: digit | '(' expr ')' | '-' factor;
digit: '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '0';
%%
