#include <stdio.h>
#include <stdlib.h>
#include "calc3.h"
#include "y.tab.h"
#define MAXSIZE 10000
int loopSize = 0;
int breakStack[MAXSIZE] = {0};
int	continueStack[MAXSIZE] = {0};	 

int temp;	
int ex(nodeType *p) {
    int i;
    if (!p) {
		return 0;
	}
	if(loopSize != 0 && breakStack[loopSize] == 1){	
		return 0;
	}
	if(loopSize != 0 && continueStack[loopSize] == 1){
		return 0;
	}	
    switch(p->type) {
    case typeCon:       return p->con.value;
    case typeId:        return sym[p->id.i];
    case typeOpr:
        switch(p->opr.oper) {
	case FOR:	
			ex(p->opr.op[0]);
			loopSize = loopSize + 1;	
			while (breakStack[loopSize] == 0 && ex(p->opr.op[1])){	
			    continueStack[loopSize] = 0;	
				ex(p->opr.op[3]);
			    continueStack[loopSize] = 0;	
				ex(p->opr.op[2]);
			    continueStack[loopSize] = 0;	
			}
			continueStack[loopSize] = 0;	
			breakStack[loopSize] = 0;
			loopSize = loopSize - 1;
			return 0;
	case DO:
				loopSize = loopSize + 1;
				do{
					continueStack[loopSize] = 0;
					ex(p->opr.op[0]);
					continueStack[loopSize] = 0;
				}while(breakStack[loopSize] == 0 && ex(p->opr.op[1]));
			    continueStack[loopSize] = 0;	
				breakStack[loopSize] = 0;
				loopSize = loopSize - 1;
				return 0;
	case BREAK:	
				if(loopSize == 0){
					printf("SYNTAX ERROR: break\n;");
					exit(EXIT_FAILURE);	
				}
				else{
					breakStack[loopSize] = 1;
				}
				return 0;
	
	case CONTINUE:
				if(loopSize == 0){
					printf("SYNTAX ERROR: continue\n;");
					exit(EXIT_FAILURE);	
				}	
				else{
					continueStack[loopSize] = 1;
				}
				return 0;
        case WHILE: 
					loopSize = loopSize + 1;		
			while(breakStack[loopSize] == 0 && ex(p->opr.op[0])){ 
					continueStack[loopSize] = 0;	
					ex(p->opr.op[1]);
					continueStack[loopSize] = 0;	
			}
					continueStack[loopSize] = 0;
					breakStack[loopSize] = 0;
					loopSize = loopSize - 1;	
					return 0;
        case IF:        if (ex(p->opr.op[0]))
                            ex(p->opr.op[1]);
                        else if (p->opr.nops > 2)
                            ex(p->opr.op[2]);
                        return 0;
        case PRINT:     printf("%d\n", ex(p->opr.op[0])); return 0;
	case READ:	printf("? "); scanf("%d", &i);
			return sym[p->opr.op[0]->id.i] = i;
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case '=':       return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
        case UMINUS:    return -ex(p->opr.op[0]);
        case '+':       return ex(p->opr.op[0]) + ex(p->opr.op[1]);
        case '-':       return ex(p->opr.op[0]) - ex(p->opr.op[1]);
        case '*':       return ex(p->opr.op[0]) * ex(p->opr.op[1]);
        case '/':       return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        case '%':       return ex(p->opr.op[0]) % ex(p->opr.op[1]);
        case '<':       return ex(p->opr.op[0]) < ex(p->opr.op[1]);
        case '>':       return ex(p->opr.op[0]) > ex(p->opr.op[1]);
        case GE:        return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
        case LE:        return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
        case NE:        return ex(p->opr.op[0]) != ex(p->opr.op[1]);
        case EQ:        return ex(p->opr.op[0]) == ex(p->opr.op[1]);
	case AND:	return ex(p->opr.op[0]) && ex(p->opr.op[1]);
	case OR:	return ex(p->opr.op[0]) || ex(p->opr.op[1]);
        }
    }
    return 0;
}
