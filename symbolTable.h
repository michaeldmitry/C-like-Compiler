#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "arraylist.h"

int getIndex(ArrayList *list, char *iden);
nodeType* search(ArrayList *list, char *iden, int extra);
nodeType* declare(ArrayList *list, char *identifier, typeEnum type, int no , int scope, FILE * out);
typeEnum getType(nodeType* x);
nodeType* createOperands(ArrayList *list, int noOperands, int op, nodeType* operand1, nodeType* operand2, int no, FILE* out);
nodeType* createConstant(ArrayList *list, char* val, typeEnum type);
nodeType* createIden(ArrayList *list, char* val,int no, FILE* out);
nodeType* createExpr(ArrayList *list, char* iden, nodeType* expr, int no, FILE* out);
void printTable(ArrayList *list, int scope, FILE* out);
void printLine(nodeType *temp, int no);
void printExpr(nodeType* expr);
conNodeType* executeOper(nodeType* expr, FILE* out);


#endif