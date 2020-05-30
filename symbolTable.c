#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "arraylist.h"
#include "symbolTable.h"
#include "y.tab.h"

int getIndex(ArrayList *list, char *iden){
    nodeType* temp=NULL;
    for(int i=0;i<list->length;i=i+1){
        temp= ArrayList_get( list, i);
        if(temp==NULL)
            break;
        if(strcmp(temp->id.name,iden)==0)
            return i;
    }
    return -1;
}
nodeType* search(ArrayList *list, char *iden, int extra){
    // do{
    if(list==NULL)
        return NULL;
            nodeType* temp=NULL;
            for(int i=0;i<list->length;i=i+1){
                temp= ArrayList_get( list, i);

                // printf("i: %d ",strcmp(temp->id.name,iden));
                if(temp==NULL)
                    break;
                if(strcmp(temp->id.name,iden)==0){

                    return temp;
                }
            
            }
            
    if(extra==0){
    search(list->prev, iden,extra);
    }else
    return NULL;
}
nodeType* declare(ArrayList *list, char *identifier, typeEnum type , int no, int scope, FILE * out){
    // printf("declare  %s\n", identifier);
    nodeType* var=NULL;
    var= search(list, identifier,1);

    if(var !=NULL){
        fprintf(out,"Warning in line %d, you already declared a varible (%s) with the same name before\n",no, var->id.name);
        // printf("%s\n",var->id.name);
        int ind= getIndex(list,identifier);
        var->id.type=type;
        var -> id.scope=scope;
        ArrayList_set(list,ind,var);
        return var;
    }
    nodeType* identifier2= malloc(sizeof(nodeType));
    identifier2->type=Id;
    identifier2->id.name=strdup(identifier);
    identifier2->id.type=type;
    identifier2->id.scope=scope;
    identifier2->id.used=false;
    identifier2->id.actual_val=NULL;
    identifier2->id.value=false;
    identifier2->id.declaration=true;
        identifier2->id.used_count=0;

    ArrayList_add( list, identifier2 );
    // free(identifier2);
    return identifier2;
}

typeEnum getType(nodeType* x){
    switch(x->type){

        case Con: return x->con.type;
        case Id:  return x->id.type;
        case Opr: return getType(x->opr.op[0]);
            
    }
}


nodeType* createOperands(ArrayList *list, int noOperands, int op, nodeType* operand1, nodeType* operand2, int no, FILE* out){

        nodeType * operand= malloc(sizeof(nodeType));
        operand->type=Opr;
        operand->opr.nops=noOperands;
        operand->opr.oper=op;
        operand-> opr.op[0]=operand1;
        if(noOperands>1){
            operand->opr.op[1]=operand2;
            if(getType(operand1)!= getType(operand2)){
                fprintf(out,"Semantic error mismatch type in line no %d \n",no);
                return NULL;
            }
        }   
        // free(operand);
        return operand;
}

nodeType* createConstant(ArrayList *list, char* val, typeEnum type){
    nodeType * constant= malloc(sizeof(nodeType));
    constant->type=Con;
    constant->con.type=type;
    constant->con.value=val;
    // free(constant);
    return constant;
}
nodeType* createIden(ArrayList *list, char* val, int no, FILE* out){
    nodeType* var=NULL;
    var= search(list, val,0);
    if(var ==NULL){
        fprintf(out,"Semantic Error: Undeclared var in line no %d\n",no);
        return NULL;
    }else{
        if(var->id.used==false){
        fprintf(out, "Semantic Error: Uninitialized var used in line no %d\n",no);
        return NULL;
        }
    return var;

    }


}
conNodeType* executeOper(nodeType* expr, FILE* out){
    if(expr==NULL && expr->type!=Opr)
        return NULL;
    conNodeType* res1=NULL;
    res1=malloc(sizeof(conNodeType));
    nodeType* operand1= expr->opr.op[0];
    switch(operand1->type){
        case Id:  
                    res1->type=operand1->id.type;
                    res1->value=operand1->id.actual_val;
                    break;
        case Con:
        //  printf("first operand is con\n");
                res1->type=operand1->con.type;
                 res1->value=operand1->con.value;
                // printf("res1 con: %s\n", res1->value);

                    break;
        case Opr:   
        // printf("first operand is expr\n"); 
        res1= executeOper(operand1,out); 
        // printf("res1 opr: %s\n", res1->value);
        
    }
    conNodeType* res2=NULL;
    res2=malloc(sizeof(conNodeType));
        nodeType* operand2= expr->opr.op[1];

    if(expr->opr.nops>1){
        switch(operand2->type){
            case Id:  
                        res2->type=operand2->id.type;
                        res2->value=operand2->id.actual_val;
                        break;
            case Con: 
            // printf("second operand is con\n");
                        res2->type=operand2->con.type;
                        res2->value=operand2->con.value;
                                        // printf("res2 con: %s\n", res2->value);
                        break;
            case Opr:
            //  printf("second operand is expr\n"); 
              res2= executeOper(operand2,out);
                // printf("res2 opr: %s\n", res2->value);

        }

    }else{
        if(strcmp(res1->value,"true")==0)
            res1->value="false";
        else
        {
                res1->value="true";
        }
        
        return res1;
    }
    conNodeType* finalRes=malloc(sizeof(conNodeType));
    if(res1->type==Int){
        finalRes->type=Int;
        int temp;
        char x[20];
        int g,k;

        switch(expr->opr.oper){

            case 0:         
                    sscanf(res1->value, "%d", &g);
                    sscanf(res2->value, "%d", &k);
                    temp=g+k;
                    sprintf(x, "%d", temp);
                    // printf("%s %s\n",res1->value, res2->value);
                    finalRes->value= malloc(sizeof(char)*20);
                    strncpy(finalRes->value,x,20);
                    break;
            case 1: 
                    sscanf(res1->value, "%d", &g);
                     sscanf(res2->value, "%d", &k);
                    //  printf("%s %s\n",res1->value, res2->value);
                    temp= g-k; //subtraction
                    sprintf(x, "%d", temp);
                    finalRes->value= malloc(sizeof(char)*20);
                    strncpy(finalRes->value,x,20);
                    break;

            case 2:
                    sscanf(res1->value, "%d", &g);
                     sscanf(res2->value, "%d", &k);
                     temp= g*k; 
                                        //  printf("%s %s\n",res1->value, res2->value);
                    sprintf(x, "%d", temp);
                    finalRes->value= malloc(sizeof(char)*20);
                    strncpy(finalRes->value,x,20);
                    break;
            case 3: 
                    sscanf(res1->value, "%d", &g);
                     sscanf(res2->value, "%d", &k);
                    // printf("%s %s\n",res1->value, res2->value);
                     if(k==0){
                         fprintf(out, "Runtime error: Division by zero\n");
                         return NULL;
                     }
                    temp= g / k; 
                    sprintf(x, "%d", temp);
                    finalRes->value= malloc(sizeof(char)*20);
                    strncpy(finalRes->value,x,20);
                                        break;
            // rest of possible cases
            default: fprintf(out, "Semantic error: Int can't perform this operation\n");
                        return NULL;
        }
    }
    else if(res1->type==Bool){
        ///same as above
        finalRes->type=Bool;
        // bool s1,s2;
        switch(expr->opr.oper){
            case 4: finalRes->value= "Bool expr";
                        break;
            case 5:finalRes->value= "Bool expr";
                        break;
            case 7:finalRes->value= "Bool expr";
                        break;
            case 8:finalRes->value= "Bool expr";
                        break;
            case 9:finalRes->value= "Bool expr";
                        break;
            case 10:finalRes->value= "Bool expr";
                        break;
            case 11:finalRes->value= "Bool expr";
                        break;
            case 12: finalRes->value= "Bool expr";
                        break;
            default: fprintf(out, "Semantic error: Bool can't perform this operation\n");
                        return NULL;
        }
    }
    else if(res1->type==String){
        fprintf(out, "Semantic error: String can't perform this operation\n");
        return NULL;
    }
    else if(res1->type==Float){
        finalRes->type=Float;
        float x1=0.0,x2=0.0;
        float j;
        char x[20];
        switch(expr->opr.oper){

            case 0:
                    // printf("%s\n",res1->value);
                    // printf("%s\n",res2->value);
                    sscanf(res1->value, "%g", &x1);
                    sscanf(res2->value, "%g", &x2);
                    // printf("%g\n",x1);
                    // printf("%g\n",x2);
                     j=x1+x2;
                    sprintf(x, "%g", j);
                    finalRes->value= malloc(sizeof(char)*20);
                    strncpy(finalRes->value,x,20);
                                        break;

            case 1: sscanf(res1->value, "%g", &x1);
                    sscanf(res2->value, "%g", &x2);
                     j=x1-x2;
                    sprintf(x, "%g", j);
                    finalRes->value= malloc(sizeof(char)*20);
                    strncpy(finalRes->value,x,20);
                                        break;
             case 2: sscanf(res1->value, "%g", &x1);
                    sscanf(res2->value, "%g", &x2);
                     j=x1*x2;
                    sprintf(x, "%g", j);
                    finalRes->value= malloc(sizeof(char)*20);
                    strncpy(finalRes->value,x,20);
                                        break;  
            case 3: sscanf(res1->value, "%g", &x1);
                    sscanf(res2->value, "%g", &x2);
                    if(x2==0.0){
                         fprintf(out, "Runtime error: Division by zero\n");
                         return NULL;
                    }
                     j=x1/x2;
                    sprintf(x, "%g", j);
                    finalRes->value= malloc(sizeof(char)*20);
                    strncpy(finalRes->value,x,20);
                                        break;                                     
            default: fprintf(out, "Semantic error: Float can't perform this operation\n");
                        return NULL;
        }
    }
    else{
                        fprintf(out, "Semantic error: Unsupported datatype\n");
                         return NULL;
    }
         
                    // printf("here\n");

    // printf(" op1: %s\n",res1->value);
    // printf(" op2: %s\n",res2->value);
    // printf("final res %s\n", finalRes->value);
    //     printf("final res %d\n", finalRes->type);

    // printf("1");
    free(res1);
    free(res2);
    return finalRes;
    
}


void printExpr(nodeType* expr){
    if(expr==NULL)
        return;
    // printf("%d\n",expr->opr.nops);
    for(int i=0;i<expr->opr.nops;i=i+1){
        // if(expr->opr.op[i] ==NULL){
            // printf("Operand %d is expr\n",i);
        // }
        if(expr->opr.op[i]->type==Con){
            printf("OperandC %d: %s\n",i,expr->opr.op[i]->con.value);
            // if(i==0)
                // printf("operation: %d\n",expr->opr.oper);
        }
        else if(expr->opr.op[i]->type==Id){
            printf("OperandI %d: %s\n",i,expr->opr.op[i]->id.actual_val);
            if(i==0)
                printf("operation: %d\n",expr->opr.oper);
        }
        else if(expr->opr.op[i]->type==Opr){
            printf("Operand %d is an expr\n",i);
            if(i==0){
                printExpr(expr->opr.op[0]);
                printf("operation: %d\n",expr->opr.op[1]->opr.oper);
                printExpr(expr->opr.op[1]);
            }
            else{
                printExpr(expr->opr.op[0]);
                printf("operation: %d\n",expr->opr.op[0]->opr.oper);
                printExpr(expr->opr.op[1]);
            }
        }
            
    }
}

nodeType* createExpr(ArrayList *list, char* iden, nodeType* expr, int no, FILE* out){
    // typeEnum type= getType(expr);
    nodeType* identifier= search(list, iden,0);
    if(identifier==NULL){
        fprintf(out, "Semantic Error: Undeclared identifier in line no %d \n",no);
        return NULL;
    }
    if(getType(expr)!=getType(identifier)){
        fprintf(out,"Semantic Error: Type mismatch in line no %d \n",no);
        return NULL;
    }
    int ind= getIndex(list,iden);
    identifier->id.used=true;
    // identifier->id.declaration=false;
    // identifier->id.used_count++;
    // printf("%d\n",expr->type);
    if(expr->type == Con){
        identifier->id.actual_val=expr->con.value;
    }
    else if(expr->type == Id){
        identifier->id.actual_val=expr->id.actual_val;
    }
    else if(expr->type == Opr){
    //     // printf(expr->opr.op, expr->opr.op[0])   
        conNodeType* resss = NULL;
        int count=0;
        resss= executeOper(expr,out);
        if(resss==NULL)
            return NULL;
        identifier->id.actual_val=malloc(sizeof(char)*20);
        strncpy(identifier->id.actual_val, resss->value,20);

        // identifier->id.actual_val=resss->value;
        // printf("%s\n",identifier->id.actual_val);

    }
    ArrayList_set(list,ind,identifier);
    return identifier;

}

void printTable(ArrayList *list, int scope, FILE* out){
    if(list->length==0)
    return;
    FILE * yyoutSymbol0= fopen("symbolTableOut.txt","w");
    FILE * yyoutSymbol= fopen("symbolTableOut.txt","a");
    nodeType* temp=NULL;
    fprintf(yyoutSymbol,"                    SYMBOL TABLE (%d) \n", scope);
    for(int i=0;i<list->length;i=i+1){
        temp= ArrayList_get( list, i);
        fprintf(yyoutSymbol,"Row %d: type:%d scope:%d name:%s used:%s value: %s\n", i,temp->id.type, temp->id.scope, temp->id.name, temp->id.used? "true":"false" , temp->id.actual_val);
    }

    for(int i=0;i<list->length;i=i+1){
        temp= ArrayList_get( list, i);
        if(temp->id.actual_val==NULL){
            fprintf(out,"Warning: unused variable %s\n",temp->id.name);
        }
    }
    // printTable(list->prev);
}

void printLine(nodeType *temp, int no){

        printf("line number %d: typee:%d type:%d scope:%d name:%s used:%s value: %s\n", no, temp->type, temp->id.type, temp->id.scope, temp->id.name, temp->id.used? "true":"false" , temp->id.actual_val);
    // }
}