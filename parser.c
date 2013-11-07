#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "parser.h"

char* traceEntry;
struct inst instructions[250];

void stringFromFile(char* fileName){
	FILE *ifp;
	traceEntry = malloc(1000*sizeof(char));
	ifp = fopen(strcat("./", fileName) ,"r");
	while (fgets(traceEntry, 1000, ifp) != NULL){
		fprint("appended string: %s \n", traceEntry);
		} 
	fclose(ifp);
	
}

void generateInstructions(){
	//int arraySize = strlen(traceEntry)/6;
	int i = 0;
	char* token;
	token= strtok(traceEntry, " ,$\n");
	while((token != NULL)){
		instructions[i].op = getInstNumber(token);
		
		token= strtok(NULL," ,\n");
		instructions[i].a = getInstNumber(token);
		
		token= strtok(NULL," ,\n");
		instructions[i].b = getInstNumber(token);
		
		token= strtok(NULL," ,\n");
		instructions[i].c = getInstNumber(token);
		
		token= strtok(NULL," ,\n");
		
		i++;
	}
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
		
	