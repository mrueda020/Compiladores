%{
#include <stdio.h>
#include <math.h>

int yylex(void);
void yyerror (char const *);
%}

%define api.value.type {double}
%token NUM

%%

input:
    %empty
    | input line
    ;

line:
    '\n'
    | exp  '\n' {printf("%.10g\n",$1);}
    ;
exp:
       NUM           { $$ = $1;           }
     | exp exp '+'   { $$ = $1 + $2;      }
     | exp exp '-'   { $$ = $1 - $2;      }
     | exp exp '*'   { $$ = $1 * $2;      }
     | exp exp '/'   { $$ = $1 / $2;      }
     | exp exp '^'   { $$ = pow ($1, $2); }  /* Exponentiation */
     | exp 'n'       { $$ = -$1;          }  /* Unary minus    */
     ;

%%
#include <ctype.h>
int yylex (void){
       int c;

       /* Skip white space.  */
       while ((c = getchar ()) == ' ' || c == '\t')
         continue;
       /* Process numbers.  */
       if (c == '.' || isdigit (c))
         {
           ungetc (c, stdin);
           scanf ("%lf", &yylval);
           return NUM;
         }
       /* Return end-of-input.  */
       if (c == EOF)
         return 0;
       /* Return a single char.  */
       return c;
     }


void yyerror (char const *s){
    int i = 0;
    //for(i=0;i<100;i++)
        printf("Hay un error en la expresion\n");
}


int main (void){

    
    return yyparse ();
}


