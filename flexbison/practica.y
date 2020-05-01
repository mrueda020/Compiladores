%{
	#include<stdio.h>
	void yyerror(char * mensaje){
		printf("error: %s\n", mensaje);
	}
%}

%token ENTER MAS ENTERO MENOS
%%
programa: linea
| programa linea
;
linea: expresion ENTER	{ printf("R=%d\n", $1);}
| ENTER
;
expresion: expresion MAS ENTERO 	{ $$ = $1+$3;}
|expresion MENOS ENTERO 	{ $$ = $1-$3;}
| ENTERO							{ $$ = $1;}
;
%%
int main (void){
	return yyparse();
	return 0;
}
