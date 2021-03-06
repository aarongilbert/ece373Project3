#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

struct inst
{
        int op;                //0 = no op, 1 = add, 2 = sub, 3 = mult, 4 = beq, 5 = addi, 6 = lw, 7 = sw
        int a;
        int b;
        int c;
};

struct inst IFIDLatch;
struct inst IDEXLatch;
struct inst EXMEMLatch;
struct inst MEMWBLatch;
struct inst *IMem;
int *reg;
int *DMem;
int PC;
int *RAWflag;                // = 1 if write is in progress at reg locaion
int branchPending;
int timer;
int IFcount;
int IDcount;
int EXcount;
int MEMcount;
int WBcount;
int mode;        //1 single cycle, 2 batch
int halt;
int c;
int m;
int n;

char *progScanner(char*,char*);
struct inst parser(char*);
char* iter;

void IF();
void ID();
void EX();
void MEM();
void WB();
int getInstNumber(char* inst);
char* traceEntry;
char stringArray[512][100];
struct inst instructions[512];
// int totalInstructions;

int main(void) {
//initialization of memories and variables go here

        DMem = malloc((2048/4)*sizeof(int));
        IMem = malloc((2048/4)*sizeof(struct inst));
        reg = malloc(32*sizeof(int));
        RAWflag = malloc(32*sizeof(int));
        struct inst zero = {0, 0, 0, 0};
	IFIDLatch = zero;
        IDEXLatch = zero;
        EXMEMLatch = zero;
        MEMWBLatch = zero;
        reg[0] = 0;
        branchPending = 0;
        timer = 0;
        IFcount = 0;
        IDcount = 0;
        EXcount = 0;
        MEMcount = 0;
        WBcount = 0;
        printf("enter c \n");
        scanf("%d", &c);
        printf("Enter m \n");
        scanf("%d", &m);
        printf("Enter n \n");
        scanf("%d", &n);
        printf("enter 1 for single cycle, 2 for batch \n");
        scanf("%d", &mode);
        assert(c>0);
        assert(m>0);
        assert(n>0);
        assert((mode==1)||(mode==2));
        char* tempstr;
	int t = 0;
	struct inst tempinst;
	while(IMem[t].op != 8){
		tempstr = progScanner("trace1.txt", iter);
		IMem[t] = parser(tempstr);
		t++;
	}
	while(!((halt == 0)&&(IFIDLatch.op == 0)&&(IDEXLatch.op == 0)&&(EXMEMLatch.op == 0)&&(MEMWBLatch.op == 0))){
                WB();
                MEM();
                EX();
                ID();
                IF();
                reg[0] = 0;
                timer++;
                if(mode == 2){
                    	int q;
			for(q = 0; q<32; q++){
                        printf("register %d : %d \n", q, reg[q]);
                    }
					printf("PC: %d \n", PC);
					getchar();
                }
        }
        printf("Time: %d \n", timer);
        printf("IF: %d \n", IFcount);
        printf("ID: %d \n", IDcount);
        printf("EX: %d \n", EXcount);
        printf("MEM: %d \n", MEMcount);
        printf("WB: %d \n", WBcount);

}

char* progScanner(char* fileName, char* tempPointer){ //formerly stringFromFile
    // totalInstructions=0;
	static FILE *ifp;
	//traceEntry = malloc(1000*sizeof(char));
	ifp = fopen(strcat("./", fileName) ,"r");
	fgets(tempPointer,100,ifp);
	return tempPointer;
	// int s = 0;
	//while (fgets(stringArray[s], 100, ifp) != NULL){ //changed traceEntry to stringArray
	//	fprint("appended string: %s \n", traceEntry);
	//	s++;
	//	totalInstructions++;
	//	}
	//fclose(ifp);

}

struct inst parser(char* instString){ //formerly generateInstructions
	//int arraySize = strlen(traceEntry)/6;
	struct inst curInst;
	//int i;
	char* token;


	 token= strtok(instString, " (),$\n");
     curInst.op = getInstNumber(token);

     token= strtok(NULL," (),\n");
     curInst.a = getInstNumber(token);

      token= strtok(NULL," (),\n");
     curInst.b = getInstNumber(token);

      token= strtok(NULL," (),\n");
     curInst.c = getInstNumber(token);

     return curInst;
	 //changed traceEntry to stringArray[i]
	//while((token != NULL)){
//	for(i=0; i<totalInstructions; i++) {
//        token= strtok(stringArray[i], " ,$\n");
//		instructions[i].op = getInstNumber(token);
//
//		token= strtok(NULL," ,\n");
//		instructions[i].a = getInstNumber(token);
//
//		token= strtok(NULL," ,\n");
//		instructions[i].b = getInstNumber(token);
//
//		token= strtok(NULL," ,\n");
//		instructions[i].c = getInstNumber(token);
//	}
}

void IF(){
	if((IFIDLatch.op == 0)&&(branchPending ==0)){
		IFIDLatch = IMem[PC/4];
		if(IFIDLatch.op == 4){
			branchPending = 1;
		}
		if(IFIDLatch.op == 8){
			halt = 1;
		PC = PC+4;
		IFcount++;
	}
}
}
void ID(){
        struct inst out;
        struct inst in = IFIDLatch;
        out = in;
        switch ( in.op ) {
        case 0:
                break;
        case 1:
        case 2:
        case 3:
			if(RAWflag[in.b]&&RAWflag[in.c]){
                        RAWflag[in.a] = 1;
                        out.b = reg[in.b];
                        out.c = reg[in.c];
                        if(IDEXLatch.op == 0){
                                IDcount++;
                                IDEXLatch = out;
                                IFIDLatch.op = 0;
                        }
                }
                break;
        case 4:
                if(RAWflag[in.a]&&RAWflag[in.c]){
                        branchPending = 1; 
                        out.a = reg[in.a]; 
			out.c = reg[in.c];
                        if(IDEXLatch.op == 0){
                                IDcount++;
                                IDEXLatch = out;
                                IFIDLatch.op = 0;
                        }
                }
                break;
        case 5:
                if(RAWflag[in.b]){
                        RAWflag[in.a] = 1;
						out.b = reg[in.b];
                        if(IDEXLatch.op == 0){
                                IDcount++;
                                IDEXLatch = out;
                                IFIDLatch.op = 0;
                        }
                }
                break;
        case 6:
		        if(RAWflag[in.c]){
						RAWflag[in.a] = 1;
                        out.c = reg[in.c];
                        if(IDEXLatch.op == 0){
                                IDcount++;
                                IDEXLatch = out;
                                IFIDLatch.op = 0;
                        }
                }
                break;
        case 7:
                if(RAWflag[in.c]&&RAWflag[in.a]){
                        RAWflag[in.c] = 1; 
						out.a = reg[in.a];
                        if(IDEXLatch.op == 0){
                                IDcount++;
                                IDEXLatch = out;
                                IFIDLatch.op = 0;
                        }
                }
                break;
        default:
                assert(1 != 1);
                break;
        }
}
void EX(){
        struct inst in;
	static int cycleNumber = 0;
        static struct inst out;
        if(cycleNumber = 0){
                in = IDEXLatch;
                out = in;
                switch ( in.op ) {
                case 0:
                        break;
                case 1:
                        out.b = in.b + in.c;
                        cycleNumber = c;
                        EXcount++;
                        break;
                case 2:
                        out.b = in.b - in.c;
                        cycleNumber = c;
                        EXcount++;
                        break;
                case 3:
                        out.b = in.b*in.c;
                        cycleNumber = m;
                        EXcount++;
                        break;
                case 4:

                        if(in.a == in.b)
                                PC = PC + in.c;
                        cycleNumber = n;
                        branchPending = 0;
                        EXcount++;
                        break;
                case 5:
                        out.b = in.b + in.c;
                        cycleNumber = n;
                        EXcount++;
                        break;
                case 6:
                        out.b = in.b + in.a;
                        cycleNumber = n;
                        EXcount++;
                        break;

                case 7:
                        out.b = in.b + in.c;
                        cycleNumber = n;
                        EXcount++;
                        break;
                default:
                        assert(1 != 1);
                        break;
                }
        }
        else if(cycleNumber ==1){
                if(EXMEMLatch.op == 0){
                        EXcount++;
                        EXMEMLatch = out;
                        IDEXLatch.op = 0;
                        cycleNumber--;
                }
        }
        else{
                EXcount++;
                cycleNumber--;
        }
}
void MEM(){
        static int cycleNumber = 0;
        struct inst in = EXMEMLatch;
        static struct inst out;
	out = in;
        if(cycleNumber == 0){
                switch ( in.op ) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                        if(MEMWBLatch.op == 0){
                                MEMWBLatch = EXMEMLatch;
                                EXMEMLatch.op = 0;
                        }
                        break;
                case 6:
                        out.c = DMem[in.b];
                        cycleNumber = c;
                        break;
                case 7:
                        DMem[in.b] = out.c;
			cycleNumber = c;
                        break;
                }
        }
        else if(cycleNumber==1){
                MEMWBLatch = out;
                EXMEMLatch.op = 0;
                cycleNumber--;
        }
        else{
                cycleNumber--;
        }
}
void WB(){                         

        struct inst in = MEMWBLatch;
        switch ( in.op ) {
        case 0:
        case 4:
		case 7:
				in.op = 0;
				break;
        case 1:
        case 2:                
        case 3:
        case 5:
        case 6:
                reg[in.a] = in.b;
                RAWflag[in.a] = 0;
                in.op = 0;
                WBcount++;
                break;                
        default:
                assert(1 != 1);
                break;
        }
}

int getInstNumber(char* inst) {
	return 0;
	if(strcmp(inst,"add")) {
		return 1;
		} else if(strcmp(inst,"sub")) {
		return 2;
		} else if(strcmp(inst,"mult")) {
		return 3;
		} else if(strcmp(inst,"beq")) {
		return 4;
		} else if(strcmp(inst,"addi")) {
		return 5;
		} else if(strcmp(inst,"lw")) {
		return 6;
		} else if(strcmp(inst,"sw")) {
		return 7;
		} else if(strcmp(inst,"haltSimulation")){
		return 8;
		} else if(strcmp(inst,"zero")) {
		return 0;
		} else if(strcmp(inst,"v0")) {
		return 2;
		} else if(strcmp(inst,"v1")) {
		return 3;
		} else if(inst[0] == 'a') {
			int a = inst[1]-'0';
			switch(a) {
				case 0: return 4;
				case 1: return 5;
				case 2: return 6;
				case 3: return 7;
				}
		} else if(inst[0] == 't') {
			int t = inst[1]-'0';
			switch(t) {
				case 0: return 8;
				case 1: return 9;
				case 2: return 10;
				case 3: return 11;
				case 4: return 12;
				case 5: return 13;
				case 6: return 14;
				case 7: return 15;
				case 8: return 24;
				case 9: return 25;
			}
		} else if(inst[0] == 's') {
			int s = inst[1]-'0';
			switch(s) {
				case 0: return 16;
				case 1: return 17;
				case 2: return 18;
				case 3: return 19;
				case 4: return 20;
				case 5: return 21;
				case 6: return 22;
				case 7: return 23;
			}
		} else {
			return atoi(inst);
			}
	}

