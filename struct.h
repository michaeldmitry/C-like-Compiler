#include <stdbool.h>

typedef enum { Con, Id, Opr } nodeEnum;
typedef enum { Int, Float, Char, Bool, String, ConstInt, ConstFloat, ConstChar, ConstBool, ConstString ,noType} typeEnum;

/* constants */
typedef struct {
	typeEnum type;
	char* value;
} conNodeType;

/* identifiers */
typedef struct {
    typeEnum type;
	int scope;
	char* name;
	bool used;
	bool value;
    char* actual_val;
	bool declaration;
    	int used_count;
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */	
    struct nodeTypeTag *op[1];	/* operands, extended at runtime */
} oprNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */

    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
    };
} nodeType;
