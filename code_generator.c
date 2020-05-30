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
static nodeType* s;
static int switchLabel=-1;
bool firstCase=true;

int execute(nodeType* line, FILE * yyout, ArrayList* currentScopePointer, int flag, int * label){
	if (line == NULL && flag != 2 && flag != 7 && flag != 5 &&flag!=9 &&flag!=11)
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

				nodeType* tempId = search(currentScopePointer, line->id.name,0);
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
				nodeType* tempId = search(currentScopePointer, line->id.name,0);
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
			// if( flag == 5){
				// fprintf(yyout, "L%03d\n", label_num);
				// label_num++;
			// }
			switch(line->opr.oper){
				case 0:
					if(line->opr.nops >= 2){
						int opp1;
						int opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								// printf("add opr1: %d\n", opp1);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:

								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								// printf("add opr2: %d\n", op22);

								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
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
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
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
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
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
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
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
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
						}
					fprintf(yyout, "AND R%d, R%d, R%d\n", opp1, opp2, registers);

					// if(flag == 4){
					// 	fprintf(yyout, "JNZ L%03d\n", label_num);

					// 	*label=label_num;

					// 	// fprintf(yyout, "JMP L%03d\n", *label-1);
					// 	// fprintf(yyout, "L%03d\n", *label);
					// }
					// else{
					// fprintf(yyout, "JNZ L%03d\n", label_num);
					// *label=label_num;
					// }
					label_num++;

					return registers++;
					}
				case 5:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer,1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
						}
					fprintf(yyout, "OR R%d, R%d, R%d\n", opp1, opp2, registers);
					// if(flag == 4){
					// 	fprintf(yyout, "JNZ L%03d\n", label_num);

					// 	*label=label_num;

					// 	// fprintf(yyout, "JMP L%03d\n", *label-1);
					// 	// fprintf(yyout, "L%03d\n", *label);
					// }
					// else{
					// fprintf(yyout, "JNZ L%03d\n", label_num);
					// *label=label_num;
					// }
					label_num++;
					return registers++;
					}
				case 6:
					if(line->opr.nops  ==1){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						// switch(line->opr.op[1]->type){
						// 	case Con:
						// 		opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
						// 		//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
						// 		break;
						// 	case Id:
						// 		opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
						// 		break;
						// 	case Opr:
						// 		opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
						// 		break;
						// }
					fprintf(yyout, "NEG R%d, R%d, R%d\n", opp1, 0, registers);
					// if(flag == 4){
					// 	fprintf(yyout, "JNZ L%03d\n", label_num);

					// 	*label=label_num;

					// 	// fprintf(yyout, "JMP L%03d\n", *label-1);
					// 	// fprintf(yyout, "L%03d\n", *label);
					// }
					// else{
					// fprintf(yyout, "JNZ L%03d\n", label_num);
					// *label=label_num;
					// }
					label_num++;
					return registers++;
					}
				case 7:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
						}
					fprintf(yyout, "EQU R%d, R%d, R%d\n", opp1, opp2, registers);
					if(flag == 4){
						fprintf(yyout, "JNZ L%03d\n", label_num);

						*label=label_num;

						// fprintf(yyout, "JMP L%03d\n", *label-1);
						// fprintf(yyout, "L%03d\n", *label);
					}
					else{
					fprintf(yyout, "JNZ L%03d\n", label_num);
					*label=label_num;
					}
					label_num++;
					return registers++;
					}
				case 8:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
						}
					fprintf(yyout, "NEQU R%d, R%d, R%d\n", opp1, opp2, registers);
					if(flag == 4){
						fprintf(yyout, "JNZ L%03d\n", label_num);

						*label=label_num;

						// fprintf(yyout, "JMP L%03d\n", *label-1);
						// fprintf(yyout, "L%03d\n", *label);
					}
					else{
					fprintf(yyout, "JNZ L%03d\n", label_num);
					*label=label_num;
					}
					label_num++;
					return registers++;
					}
				case 9:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
						}
					// printf("greater %d  %d  %d\n", opp1, op2, registers);

					fprintf(yyout, "GRT R%d, R%d, R%d\n", opp1, opp2, registers);


					if(flag == 4){
						fprintf(yyout, "JNZ L%03d\n", label_num);

						*label=label_num;

						// fprintf(yyout, "JMP L%03d\n", *label-1);
						// fprintf(yyout, "L%03d\n", *label);
					}
					else{
					fprintf(yyout, "JNZ L%03d\n", label_num);
					*label=label_num;
					}
					label_num++;
					return registers++;
					}
				case 10:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
						}
					fprintf(yyout, "LEST R%d, R%d, R%d\n", opp1, opp2, registers);
					if(flag == 4){
						fprintf(yyout, "JNZ L%03d\n", label_num);

						*label=label_num;

						// fprintf(yyout, "JMP L%03d\n", *label-1);
						// fprintf(yyout, "L%03d\n", *label);
					}
					else{
					fprintf(yyout, "JNZ L%03d\n", label_num);
					*label=label_num;
					}
					label_num++;
					return registers++;
					}
				case 11:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
						}
					fprintf(yyout, "LESE R%d, R%d, R%d\n", opp1, opp2, registers);
					if(flag == 4){
						fprintf(yyout, "JNZ L%03d\n", label_num);

						*label=label_num;

						// fprintf(yyout, "JMP L%03d\n", *label-1);
						// fprintf(yyout, "L%03d\n", *label);
					}
					else{
					fprintf(yyout, "JNZ L%03d\n", label_num);
					*label=label_num;
					}
					label_num++;
					return registers++;
					}
				case 12:
					if(line->opr.nops >= 2){
						int opp1,opp2;
						switch(line->opr.op[0]->type){
							case Con:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[0]->con.value);
								break;
							case Id:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp1 = execute(line->opr.op[0], yyout, currentScopePointer, 1, label);
								break;
						}
						switch(line->opr.op[1]->type){
							case Con:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								//fprintf(yyout, "value %s \n", line->opr.op[1]->con.value);
								break;
							case Id:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
							case Opr:
								opp2 = execute(line->opr.op[1], yyout, currentScopePointer, 1, label);
								break;
						}
					fprintf(yyout, "GRTE R%d, R%d, R%d\n", opp1, opp2, registers);
					if(flag == 4){
						fprintf(yyout, "JNZ L%03d\n", label_num);

						*label=label_num;

						// fprintf(yyout, "JMP L%03d\n", *label-1);
						// fprintf(yyout, "L%03d\n", *label);
					}
					else{
					fprintf(yyout, "JNZ L%03d\n", label_num);
					*label=label_num;
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
		execute(line, yyout, currentScopePointer, 0, label);
		// execute(line->opr.op[1], yyout, currentScopePointer, 1);

	}
}

if(flag==2){ //scope closure
	fprintf(yyout, "L%03d\n", (*label));
		// label_num++;

}
if(flag==9){  //while closure
		fprintf(yyout, "L%03d\n", (*label)+1);

}
if(flag == 3){	//while
	fprintf(yyout, "JMP L%03d\n", *label);
}
if(flag == 11){	//while
	fprintf(yyout, "JMP L%03d\n", *label);
}
if(flag ==10){
		fprintf(yyout, "JMP L%03d\n", *label);

}
if(flag == 5){	//scope creation
	fprintf(yyout, "L%03d\n", label_num);
		*label=label_num;
		label_num++;
}
if(flag == 6){	//for_loop
	execute(line,yyout,currentScopePointer,0, label);
	fprintf(yyout, "L%03d\n", label_num);
		*label=label_num;
	label_num++;
}
if(flag == 7){
	fprintf(yyout, "JMP L%03d\n", label_num);
	fprintf(yyout, "L%03d\n", *label);
	// label_num++;
	return label_num++;
}

if(flag == 8){
	fprintf(yyout, "JMP L%03d\n", *label);
	fprintf(yyout, "L%03d\n", *label+1);
	// label_num++;
}
}

void exec2(char* line, FILE * yyout, ArrayList* currentScopePointer, int flag){

	if(flag == 0)	{
		// fprintf(yyout, "SWITCH %s\n", line);
		nodeType* te=NULL;
		te=search(currentScopePointer, line, 0);
		s = te;
		switchLabel=label_num;
		label_num++;
		}
	if(flag == 1){
		if(firstCase==true){
			firstCase=false;
		}else{
			fprintf(yyout, "L%03d\n", label_num);
			label_num++;

		}
		nodeType* op2N=NULL;
		op2N= malloc(sizeof(nodeType));
		op2N->type=Con;
		op2N->con.type=Int;
		op2N->con.value=line;

		int op1= execute(op2N, yyout, currentScopePointer, 1,&flag);
		int op2=execute(s, yyout,currentScopePointer, 1, &flag);


		fprintf(yyout, "EQU R%d, R%d, R%d\n", op1, op2, registers);
		fprintf(yyout, "JNZ L%03d\n", label_num);
		registers++;
		// label_num++;
		// label_num;
	}
	if(flag == 2){
		fprintf(yyout, "JMP L%03d\n", switchLabel);
		// label_num++;
	}
	if(flag == 3){
		fprintf(yyout,"L%03d\n", label_num-1);
		label_num++;
	}
	if(flag == 4){
		fprintf(yyout, "L%03d\n", label_num);
		// label_num;
	}
	if(flag==5){
		fprintf(yyout,"L%03d\n",switchLabel);
		switchLabel=-1;
		firstCase=true;
		label_num++;
	}
}

void whileLabel(int * label, FILE * yyout){
	fprintf(yyout, "L%03d\n", label_num);
	*label=label_num;
	label_num++;
}
