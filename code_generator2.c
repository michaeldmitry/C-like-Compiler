#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "y.tab.h"
#include "arraylist.h"
#include "symbolTable.h"

static int label_num = 1;
static int registers = 1;
static int op1, op2;
static bool flag_else = false;
static char* s;
int execute(nodeType* line, FILE * yyout, ArrayList* currentScopePointer, int flag){
	if (line == NULL && flag != 2 && flag != 7 && flag != 5)
		return 0;
	if(flag == 0  || flag == 3 || flag == 4 || flag == 5){
	switch(line->type){

		case Con:
			// printf("herecon\n");
			// 						printf("%s\n",line->con.value);
			fprintf(yyout, "MOV R%d %s \n", registers, line->con.value);
			return registers++;
			break;
		case Id:

			if(line->id.declaration == true && line->id.used == false){

				nodeType* tempId = search(currentScopePointer, line->id.name);
				tempId->id.declaration = false;

				// tempId->id.declaration = false;
        		// ArrayList_set(currentScopePointer,ind,tempId);
				switch(line->id.type){
					case Int:
						fprintf(yyout, "Wordi %s \n", line->id.name);
						break;
					case Float:
						// printf("inside float %s\n",line->id.name);

						fprintf(yyout, "Wordf %s \n", line->id.name);
						break;
					case Char:
						fprintf(yyout, "Wordc %s \n", line->id.name);
						break;
					case Bool:
						fprintf(yyout, "Wordb %s \n", line->id.name);
						break;
					case String:
						fprintf(yyout, "Words %s \n", line->id.name);
						break;
					case ConstInt:
						fprintf(yyout, "Wordci %s \n", line->id.name);
						break;
					case ConstFloat:
						fprintf(yyout, "Wordcf %s \n", line->id.name);
						break;
					case ConstChar:
						fprintf(yyout, "Wordcc %s \n", line->id.name);
						break;
					case ConstBool:
						fprintf(yyout, "Wordcb %s \n", line->id.name);
						break;
					case ConstString:
						fprintf(yyout, "Wordcs %s \n", line->id.name);
						break;
					case noType:
						fprintf(yyout, "Wordnt %s \n", line->id.name);
						break;
				}
			}
			else if(line->id.declaration == false && line->id.used == true){
				nodeType* tempId = search(currentScopePointer, line->id.name);
				tempId->id.used_count++;
				
					// if(line->id.used_count == 1){
						fprintf(yyout, "STD R%d, %s \n", registers-1, line->id.name);
						//fprintf(yyout, "flag %d\n", flag);
						return registers;
					// }
					// else{
					
						// fprintf(yyout, "MOV1 R%d, %s\n", registers, line->id.name);
						//fprintf(yyout, "%d, %d\n", line->id.declaration, line->id.used);
						// return registers++;
					// }
			}
			break;

		case Opr:
			// printf("here\n");
			if(flag ==3 || flag == 5){
				fprintf(yyout, "L%03d\n", label_num);
				label_num++;
			}
			switch(line->opr.oper){
				case 0:
					if(line->opr.nops >= 2){
						int opp1;
						int opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								// printf("add opr1: %d\n", opp1);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:

								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								// printf("add opr2: %d\n", op22);

								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "ADD R%d, R%d, R%d\n", opp1, opp2, registers);
					return registers++;
					}
				case 1:
					if(line->opr.nops >= 2){
						int opp1,opp2;;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "SUB R%d, R%d, R%d\n", opp1, opp2, registers);
					return registers++;
					}
				case 2:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "MUL R%d, R%d, R%d\n", opp1, opp2, registers);
					return registers++;
					}
				case 3:
					if(line->opr.nops >= 2){
							int opp1,opp2;

						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "DIV R%d, R%d, R%d\n", opp1, opp2, registers);
					return registers++;
					}
				case 4:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "AND R%d, R%d, R%d\n", opp1, opp2, registers);

					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}

					return registers++;
					}
				case 5:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer,1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "OR R%d, R%d, R%d\n", opp1, opp2, registers);
					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}
					label_num++;
					return registers++;
					}
				case 6:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "NEG R%d, R%d, R%d\n", opp1, opp2, registers);
					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}
					label_num++;
					return registers++;
					}
				case 7:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "EQU R%d, R%d, R%d\n", opp1, opp2, registers);
					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}
					label_num++;
					return registers++;
					}
				case 8:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "NEQU R%d, R%d, R%d\n", opp1, opp2, registers);
					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}
					label_num++;
					return registers++;
					}
				case 9:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					// printf("greater %d  %d  %d\n", opp1, op2, registers);

					fprintf(yyout, "GRT R%d, R%d, R%d\n", opp1, opp2, registers);
					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}
					label_num++;
					return registers++;
					}
				case 10:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "LEST R%d, R%d, R%d\n", opp1, opp2, registers);
					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}
					label_num++;
					return registers++;
					}
				case 11:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "LESE R%d, R%d, R%d\n", opp1, opp2, registers);
					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}
					label_num++;
					return registers++;
					}
				case 12:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1);
								break;
						}
					fprintf(yyout, "GRTE R%d, R%d, R%d\n", opp1, opp2, registers);
					fprintf(yyout, "JNZ L%03d\n", label_num);
					if(flag == 4){
						fprintf(yyout, "JMP L%03d\n", label_num-1);
						fprintf(yyout, "L%03d\n", label_num);
					}
					label_num++;
					return registers++;
					}
				
				
			}
			
	}
}
else if (flag == 1){
	// printf("dakahlt ya basha\n");
	if(line->type == Id){
		// printf("id %s \n", line->id.name);
		fprintf(yyout, "MOV R%d, %s \n", registers, line->id.name);
			return registers++;	
	}
	else if(line->type == Con){
				// printf("con %s \n", line->con.value);

		fprintf(yyout, "MOV R%d, %s \n", registers, line->con.value);
			return registers++;	
	}
	else if(line->type ==Opr){
		execute(line, yyout, currentScopePointer, 0);
		// execute(line->opr.op[1], yyout, currentScopePointer, 1);

	}
}


if(flag == 2){ //scope closure
	fprintf(yyout, "L%03d\n", label_num-1);
	label_num++;
}
if(flag == 3){	//while
	fprintf(yyout, "JMP L%03d\n", label_num-2);
}
if(flag == 5){	//scope creation
	fprintf(yyout, "Lhere%03d\n", label_num);
		label_num++;
}
if(flag == 6){	//for_loop
	fprintf(yyout, "L%03d\n", label_num);
		label_num++;
}
if(flag == 7){
	fprintf(yyout, "JMP L%03d\n", label_num);
	fprintf(yyout, "L%03d\n", label_num-1);
	label_num++;
}

if(flag == 8){
	fprintf(yyout, "JMP L%03d\n", label_num-2);
	fprintf(yyout, "L%03d\n", label_num-1);
	label_num++;
}

}

void exec2(char* line, FILE * yyout, ArrayList* currentScopePointer, int flag){

	if(flag == 0)	{
		fprintf(yyout, "SWITCH %s\n", line);
		s = line;
		}
	if(flag == 1){
		fprintf(yyout, "L%03d\n", label_num);
		fprintf(yyout, "EQU %s, %s, R%d\n", line, s, registers+1);
		fprintf(yyout, "JNZ L%03d\n", label_num+1);
		label_num;
	}
	if(flag == 2){
		fprintf(yyout, "JMP L500\n");
		label_num++;
	}
	if(flag == 3){
		fprintf(yyout, "L500\n");
		label_num++;
	}
	if(flag == 4){
		fprintf(yyout, "L%03d\n", label_num);
		label_num;
	}
}


