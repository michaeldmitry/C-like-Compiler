%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "arraylist.h"
#include "struct.h"
void yyerror(char *s);
#include "y.tab.h"

%}

%%
if 		return IF;
else 	return ELSE;
while 	return WHILE;
for 	return FOR;
do 		return DO;
switch 	return SWITCH;
case 	return CASE;
break 	return BREAK;
default return DEFAULT;
int 	return INT;
string	 return STRING;
float 	return FLOAT;
bool 	return BOOL;
\+ 		return PLUS;
\- 		return MINUS;
\* 		return MULTIPLY;
\/ 		return DIVIDE;
\|\| 	return OR;
&& 		return AND;
! 		return NEGATION;
\=\= 		return EQUAL_R;
>\= 		return GREAT_EQU;
\<\= 		return LESS_EQU;
\< 		return LESS;
\> 		return GREAT;
!\= 		return NOT_EQU;
\=						return ASSIGN;
\(                 return OPEN_BRC;
\)               return CLOSE_BRC;
\{				return OPEN_BRACES;
\}				return CLOSE_BRACES;
\,		return COMMA;
true|false 					{yylval.string = strdup(yytext); return BOOL_V;}
return			return RETURN;
void 			return VOID;
\"[a-zA-Z0-9]+\" 				{ yylval.string= strdup(yytext); return WORD;}
:						return COLON;
; 						return SEMICOLON;
[a-zA-Z]+				{ yylval.string= strdup(yytext); return IDEN;}
[0-9]+[\.][0-9]+ 			{yylval.string= strdup(yytext); return DECIMAL;}
[0-9]+ 						{ yylval.string= strdup(yytext); 
							return NUM;}
[a-zA-Z0-9]+ {yyerror("Lexical Error");}
[ \t]+ 					;

\n 						;

						

%%

int yywrap(void){
	return 1;
}
