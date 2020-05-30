%{
#include <stdio.h>
#include "arraylist.h"
#include "struct.h"
#include "symbolTable.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


int yylex(void);
FILE * yyin;
FILE * yyout;
FILE * yyoutErrors;
FILE * yyoutCode;

int ifLabels[5];
int ifCurrent;
int ifTemp;

int whileLabels[10];
int whileCurrent;
int whileTemp;

int doLabels[10];
int doCurrent;
int doTemp;

int forLabels[10];
int forCurrent;
int forTemp;

ArrayList * symbolTable;
ArrayList* currentScopePointer;
// ArrayList*[100] TopTable;
nodeType* tempSwitch;

int yylineno;
int scope;
void yyerror(char *s);
int execute(nodeType* line, FILE * yyout, ArrayList* symboltable, int flag, int * label);
void exec2(char* line, FILE * yyout, ArrayList* currentScopePointer, int flag);
void whileLabel(int * label, FILE * yyout);

%}

%union{
    char* string;
    int val;
    nodeType* ptr;
    typeEnum keywords;
};

%start program

%token <string> IF
%token <string> ELSE
%token <string> WHILE
%token <string> FOR
%token <string> NUM
%token <string> DO
%token <string> SWITCH
%token <string> CASE
%token <string> BREAK
%token <string> DEFAULT
%token <keywords> INT
%token <keywords> STRING
%token <keywords> FLOAT
%token <keywords> BOOL
%token <string> PLUS
%token <string> MINUS
%token <string> MULTIPLY
%token <string> DIVIDE
%token <string> OR
%token <string> AND
%token <string> NEGATION
%token <string> EQUAL_R
%token <string> GREAT_EQU
%token <string> LESS_EQU
%token <string> LESS
%token <string> GREAT
%token <string> NOT_EQU
%token <string> ASSIGN
%token <string> OPEN_BRC
%token <string> CLOSE_BRC
%token <string> OPEN_BRACES
%token <string> CLOSE_BRACES
%token <string> BOOL_V
%token <string> DECIMAL
%token <string> WORD
%token <string> COLON
%token <string> SEMICOLON
%token <string> IDEN
%token <string> COMMA
%token <string> RETURN
%token <string> VOID

%type <ptr> code line if_stmt3 elseRule casestm2 switch_case2 scopeCreator vardec stmt_list while2 for_loop1 for_loop2 do_while do_while3 do_while2 stmt fun_call scopeClosure c_params c_params_list c_param expr_stmt compound_stmt otherCompund_stmt vardecs expr all_expr t arith_expr if_stmt2 if_stmt switch_case block caselist casestm defaultstm break_stmt loops while for_loop  return_stmt
%type <keywords> datatype

%right ASSIGN
%left AND OR
%left EQUAL_R NOT_EQU
%left LESS_EQU LESS GREAT GREAT_EQU
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right NEGATION

// %type <string> code line

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
program: code		{fprintf(yyout,"program: code\n"); ifCurrent=-1; ifTemp=-1; whileCurrent=-1; whileTemp=-1; forCurrent=-1; forTemp=-1; doCurrent=-1; doTemp=-1;};
code: code line {fprintf(yyout,"code: code line\n"); $$=$2; } |  line {fprintf(yyout,"code: line \n"); $$=$1;};

line: vardec		{fprintf(yyout,"line: vardec \n"); $$=$1; } 
        | stmt {fprintf(yyout,"line: stmt \n"); } 
      ;


vardec:	datatype IDEN SEMICOLON		{fprintf(yyout,"vardec: datatype(%d) IDEN(%s) SEMICOLON \n",$1,$2); 
                                        $$=declare(currentScopePointer, $2,$1, yylineno, scope,yyoutErrors);  execute($$, yyoutCode, currentScopePointer, 0,&ifTemp); }
        | error SEMICOLON {$$=NULL; fprintf(yyoutErrors,"Syntax Error in line no %d\n",yylineno);}
        | error CLOSE_BRACES {$$=NULL;  fprintf(yyoutErrors,"Syntax Error in line no %d\n",yylineno);}
        ;

datatype: INT {$$=Int;}| STRING {$$=String;} | FLOAT{$$=Float;} | BOOL{$$=Bool;};

stmt_list: stmt_list stmt {fprintf(yyout,"stmt-list: stmt_list stmt \n");} |   {fprintf(yyout,"stmt-list: Empty \n");} ;

stmt:	expr_stmt 	{fprintf(yyout,"stmt: expr_stmt \n"); execute($1, yyoutCode, currentScopePointer, 0, &ifTemp);}
        | if_stmt2 if_stmt {fprintf(yyout,"stmt: if_stmt2 if_stmt\n");} 
	| switch_case switch_case2 {fprintf(yyout,"stmt: switch_case switch_case2\n");}
        | loops  {fprintf(yyout,"stmt: loops \n");}
        | return_stmt {fprintf(yyout,"stmt: return_stmt \n");}
	| compound_stmt otherCompund_stmt {fprintf(yyout,"stmt: compound_stmt otherCompund_stmt\n");}
	| break_stmt {fprintf(yyout,"stmt: break_stmt\n");}
        | fun_call {fprintf(yyout,"stmt: fun_call\n");}
	;

fun_call: IDEN OPEN_BRC c_params CLOSE_BRC SEMICOLON {fprintf(yyout,"fun_call: IDEN OPEN_BRC c_params CLOSE_BRC SEMICOLON\n");}; 
// fun_call2: IDEN OPEN_BRC c_params CLOSE_BRC {printf("fun_call2: IDEN OPEN_BRC c_params CLOSE_BRC \n");}; 

c_params: c_params_list {fprintf(yyout,"c_params: c_params_list \n");}  |  {fprintf(yyout,"c_params: Empty \n");} ;
c_params_list: c_params_list COMMA c_param {fprintf(yyout,"c_params_list: c_params_list COMMA c_param \n");} | c_param {fprintf(yyout,"c_params_list: c_param \n");} ;
c_param: all_expr {fprintf(yyout,"c_param: all_expr \n");};

expr_stmt: expr SEMICOLON {fprintf(yyout,"expr_stmt: expr SEMICOLON\n"); $$=$1; };

scopeCreator: OPEN_BRACES { fprintf(yyout,"scopeCreator: OPEN_BRACES\n"); scope=scope+1; currentScopePointer= createNewScope(currentScopePointer, sizeof(nodeType));
        };
scopeClosure: CLOSE_BRACES {fprintf(yyout,"scopeClosure: CLOSE_BRACES \n");};

compound_stmt:	scopeCreator vardecs   {fprintf(yyout,"Compound_stmt: scopeCreator verdecs \n"); $$=$2;};
                                                                
otherCompund_stmt: stmt_list scopeClosure { fprintf(yyout,"otherCompund_stmt: stmt_list CLOSE_BRACES\n"); printTable(currentScopePointer,scope,yyoutErrors); currentScopePointer=currentScopePointer->prev; scope=scope-1;};

vardecs: vardecs vardec {fprintf(yyout,"vardecs: vardecs vardec\n"); $$=$2; } | {fprintf(yyout,"vardecs: Empty\n");} ;

expr:	IDEN ASSIGN all_expr {fprintf(yyout,"expr: IDEN(%s) ASSIGN all_expr \n",$1); $$= createExpr(currentScopePointer, $1, $3, yylineno, yyoutErrors);
                                                                     if($$==NULL)
                                                                     exit(0);
                                                                      execute($3, yyoutCode, currentScopePointer, 1, &ifTemp);
                                                                     }   
        ;
all_expr: arith_expr {fprintf(yyout,"all_expr: arith_expr\n"); $$=$1; } | t {fprintf(yyout,"all_expr: t\n"); $$=$1; } ;

t:      BOOL_V {fprintf(yyout,"t: BOOL_V(%s)\n", $1);
               $$=createConstant(currentScopePointer, $1, Bool );}
        |   NUM	{fprintf(yyout,"t: NUM(%s) \n",$1); 
        $$=createConstant(currentScopePointer, $1, Int ); }
        | WORD {fprintf(yyout,"t: WORD(%s)\n", $1);
        $$=createConstant(currentScopePointer, $1, String );}
      | DECIMAL	{fprintf(yyout,"t: DECIMAL(%s) \n",$1);
      $$=createConstant(currentScopePointer, $1, Float );}
      | IDEN  {fprintf(yyout,"t: IDEN(%s) \n",$1);
      $$=createIden(currentScopePointer, $1, yylineno ,yyoutErrors);
      if($$==NULL)
        exit(0);}
      | OPEN_BRC all_expr CLOSE_BRC {fprintf(yyout,"t: OPEN_BRC all_expr CLOSE_BRC \n"); $$=$2;}
      ;


arith_expr:
       all_expr PLUS all_expr {fprintf(yyout,"arith_expr: all_expr PLUS all_expr \n"); 
                                 $$= createOperands(currentScopePointer, 2, 0, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);} 
      | all_expr MINUS all_expr {fprintf(yyout,"arith_expr: all_expr MINUS all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 1, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);} 
      | all_expr MULTIPLY all_expr {fprintf(yyout,"arith_expr: all_expr MULTIPLY all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 2, $1, $3, yylineno, yyoutErrors); 
                                 if($$==NULL)
                                exit(0);} 
      | all_expr DIVIDE all_expr {fprintf(yyout,"arith_expr: all_expr DIVIDE all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 3, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
        | all_expr AND all_expr {fprintf(yyout,"arith_expr: all_expr AND all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 4, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
        | all_expr OR all_expr {fprintf(yyout,"arith_expr: all_expr OR all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 5, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
        | NEGATION all_expr {fprintf(yyout,"arith_expr: NEGATION all_expr\n");
                                 $$= createOperands(currentScopePointer, 1, 6, $2, NULL, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
        | all_expr EQUAL_R all_expr {fprintf(yyout,"all_expr EQUAL_R all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 7, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
        | all_expr NOT_EQU all_expr {fprintf(yyout,"all_expr NOT_EQU all_expr\n");
                                         $$= createOperands(currentScopePointer, 2, 8, $1, $3, yylineno, yyoutErrors);
                                         if($$==NULL)
                                exit(0);}
        | all_expr GREAT all_expr {fprintf(yyout,"all_expr GREAT all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 9, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
        | all_expr LESS all_expr {fprintf(yyout,"all_expr LESS all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 10, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
        | all_expr LESS_EQU all_expr {fprintf(yyout,"all_expr LESS_EQU all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 11, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
        | all_expr GREAT_EQU all_expr {fprintf(yyout,"all_expr GREAT_EQU all_expr\n");
                                 $$= createOperands(currentScopePointer, 2, 12, $1, $3, yylineno, yyoutErrors);
                                 if($$==NULL)
                                exit(0);}
      ;
// if_stmt2: IF OPEN_BRC all_expr {fprintf(yyout, "if_stmt2: IF OPEN_BRC all_expr\n"); execute($3, yyoutCode, currentScopePointer, 0, &ifTemp);};

// if_stmt:  CLOSE_BRC stmt {fprintf(yyout,"if_stmt:  CLOSE_BRC stmt \n");} %prec LOWER_THAN_ELSE
//         |  CLOSE_BRC stmt ELSE stmt {fprintf(yyout,"if_stmt: CLOSE_BRC stmt ELSE stmt \n");};

elseRule: ELSE { fprintf(yyout, "elseRule: ELSE\n"); ifTemp=execute(NULL, yyoutCode, currentScopePointer, 7, &ifLabels[ifCurrent]); ifCurrent=ifCurrent; ifLabels[ifCurrent]=ifTemp;};
if_stmt2: IF OPEN_BRC all_expr {fprintf(yyout,"if_stmt2: IF OPEN_BRC all_expr \n"); execute($3, yyoutCode, currentScopePointer, 0, &ifTemp); ifCurrent=ifCurrent+1; ifLabels[ifCurrent]=ifTemp; };
if_stmt: CLOSE_BRC stmt {  fprintf(yyout,"if_stmt: CLOSE_BRC stmt \n"); execute(NULL, yyoutCode, currentScopePointer, 2, &ifLabels[ifCurrent]); ifCurrent=ifCurrent-1;} %prec LOWER_THAN_ELSE
        | CLOSE_BRC stmt elseRule if_stmt3 {fprintf(yyout,"if_stmt: CLOSE_BRC stmt elseRule if_stmt3\n"); };
if_stmt3: stmt {fprintf(yyout,"if_stmt3: stmt \n");  execute(NULL, yyoutCode, currentScopePointer, 2, &ifLabels[ifCurrent]); ifCurrent=ifCurrent-1;};


// switch_case: SWITCH OPEN_BRC IDEN CLOSE_BRC OPEN_BRACES block scopeClosure {fprintf(yyout,"switch_case:  SWITCH OPEN_BRC ID CLOSE_BRC OPEN_BRACES block CLOSE_BRACES\n");} ;
switch_case: SWITCH OPEN_BRC IDEN { tempSwitch=createIden(currentScopePointer, $3,yylineno, yyoutErrors); if(tempSwitch==NULL){ exit(0);}  fprintf(yyout,"switch_case: SWITCH OPEN_BRC IDEN (%s)\n",$3); exec2($3, yyoutCode, currentScopePointer, 0);};
switch_case2: CLOSE_BRC OPEN_BRACES block CLOSE_BRACES { fprintf(yyout,"switch_case2: CLOSE_BRC scopeCreator block CLOSE_BRACES\n"); exec2($4,yyoutCode, currentScopePointer, 5);} ;

block: caselist {fprintf(yyout,"block: caselist\n");}| caselist defaultstm {fprintf(yyout,"block: caselist defaultstm\n");};
// caselist: casestm {fprintf(yyout,"caselist: casestm\n");} | casestm caselist {fprintf(yyout,"caselist: casestm caselist\n");};
// casestm: CASE NUM COLON stmt {fprintf(yyout,"casestm: CASR NUM COLON stmt\n");};
// defaultstm : DEFAULT COLON stmt {fprintf(yyout,"defaultstm: DEFAULT COLON stmt\n"); execute($3, yyoutCode, currentScopePointer, 0, &ifTemp);};
// break_stmt: BREAK SEMICOLON {fprintf(yyout,"break_stmt: BREAK SEMICOLON\n");};

caselist: casestm casestm2 {fprintf(yyout,"caselist: casestm casestm2\n");exec2("", yyoutCode, currentScopePointer, 4);} | casestm casestm2 caselist {fprintf(yyout,"caselist: casestm casestm2 caselist\n");};
casestm: CASE NUM COLON {fprintf(yyout,"casestm: CASE NUM COLON"); exec2($2, yyoutCode, currentScopePointer, 1);};
casestm2: stmt { fprintf(yyout,"casestm2: stmt\n");};
defaultstm : DEFAULT COLON stmt {fprintf(yyout,"defaultstm: DEFAULT COLON stmt\n");};
break_stmt: BREAK SEMICOLON {fprintf(yyout,"break_stmt: BREAK SEMICOLON\n");exec2($2, yyoutCode, currentScopePointer, 2);};



loops :  while2 while  {fprintf(yyout,"loops: while2 while \n"); execute(NULL, yyoutCode, currentScopePointer, 9, &(whileLabels[whileCurrent])); whileCurrent=whileCurrent-1; } | do_while do_while2 do_while3 {fprintf(yyout,"loops: do_while do_while2 do_while3\n");} | for_loop1 for_loop for_loop2 {fprintf(yyout,"loops: for_loop1 for_loop for_loop2\n");} ;
while2: WHILE OPEN_BRC all_expr {fprintf(yyout, "while2: WHILE OPEN_BRC all_expr \n"); whileLabel(&whileTemp, yyoutCode); whileCurrent=whileCurrent+1; whileLabels[whileCurrent]=whileTemp;  execute($3, yyoutCode, currentScopePointer, 3, &whileTemp); whileCurrent=whileCurrent+1; whileLabels[whileCurrent]=whileTemp; };
while:  CLOSE_BRC stmt  { whileCurrent=whileCurrent-1; execute(NULL, yyoutCode, currentScopePointer, 11, &whileLabels[whileCurrent]); fprintf(yyout,"while: CLOSE_BRC stmt \n");} ;

for_loop1: FOR OPEN_BRC expr {fprintf(yyout, "for_loop1: FOR OPEN_BRC expr\n"); execute($3, yyoutCode, currentScopePointer, 6, &forTemp); forCurrent=forCurrent+1; forLabels[forCurrent]=forTemp;};
for_loop:  SEMICOLON all_expr SEMICOLON  {execute($2, yyoutCode, currentScopePointer, 3, &forTemp); forCurrent=forCurrent+1; forLabels[forCurrent]=forTemp; fprintf(yyout,"for_loop: SEMICOLON all_expr SEMICOLON \n");} ;
for_loop2: expr CLOSE_BRC stmt { forCurrent= forCurrent-1; execute($3, yyoutCode, currentScopePointer, 8, &forLabels[forCurrent]); forCurrent=forCurrent-1; fprintf(yyout,"for_loop2: expr CLOSE_BRC OPEN_BRACES stmt \n");};

do_while3:all_expr CLOSE_BRC SEMICOLON {fprintf(yyout, "do_while3: all_expr CLOSE_BRC SEMICOLON\n"); execute($1, yyoutCode, currentScopePointer, 4, &doTemp); doCurrent=doCurrent+1; doLabels[doCurrent]=doTemp; doCurrent=doCurrent-1; execute($1, yyoutCode, currentScopePointer, 10, &doLabels[doCurrent]); execute($1, yyoutCode, currentScopePointer, 9, &(doLabels[doCurrent])); doCurrent=doCurrent-1;};
do_while: DO scopeCreator { whileLabel(&doTemp, yyoutCode); doCurrent=doCurrent+1; doLabels[doCurrent]=doTemp; fprintf(yyout,"do_while: DO scopeCreator\n");};
do_while2: stmt_list CLOSE_BRACES WHILE OPEN_BRC  {  execute($1, yyoutCode, currentScopePointer, 0, &ifTemp); fprintf(yyout,"do_while2: stmt_list CLOSE_BRACES WHILE OPEN_BRC \n");} ;

return_stmt: RETURN SEMICOLON {fprintf(yyout,"return_stmt: RETURN SEMICOLON\n");}
         | RETURN all_expr SEMICOLON {fprintf(yyout,"return_stmt: RETURN all_expr SEMICOLON\n");}
          ;

%%
void yyerror(char *s){
        if(strcmp(s,"Lexical Error")==0){
        fprintf(yyoutErrors,"%s in line number: %d\n", s,yylineno);
        }
}

int main(void){
        yyin = fopen("testfile.txt","r+");
        yyout= fopen("parserOut.txt","w");
        yyoutErrors= fopen("ErrorsOut.txt","w");
        yyoutCode = fopen("codeOut.txt","w");
        ifCurrent=-1;
        ifTemp=-1;
        whileCurrent=-1;
        whileTemp=-1;
        doCurrent=-1;
        doTemp=-1;
        forCurrent=-1;
        forTemp=-1;
        tempSwitch=NULL;

        // ifLabels[0]=50;
        if(yyin == NULL){
                printf("error");
        }
        else{
                symbolTable=newArrayList(sizeof(nodeType));
                currentScopePointer=symbolTable;
                scope=0;
                // printf("%zu\n",symbolTable->data_size);
                yyparse();
                // printf("parsing\n");
                // printf("%d\n",ifLabels[0]);
                printTable(symbolTable,scope,yyoutErrors);

        }
        fclose(yyin);
        return 0;
}
