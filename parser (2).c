#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

char* traceEntry;
char stringArray[512][100];
struct inst instructions[512];
// int totalInstructions;

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

int getInstNumber(char* inst) {
	int return 0;
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
		} else if(inst[0] == "a") {
			int a = inst[1]-'0';
			switch(a) {
				case 0: return 4;
				case 1: return 5;
				case 2: return 6;
				case 3: return 7;
				}
		} else if(inst[0] == "t") {
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
		} else if(inst[0] == "s") {
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

