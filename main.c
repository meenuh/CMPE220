//
// Created by dearm on 9/17/2016.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu_utils.h"

char *checkAndGetArg(int argc, char **arg);
void init();
void freeMemory();

char PC[PC_SIZE + 1];
char memAddr[WORD_SIZE + 1];
char memData[WORD_SIZE + 1];
char instrReg[WORD_SIZE + 1];
char flags[WORD_SIZE + 1];

char *memory[MEM_ROWS]; //64kb mem
char *regFile[NUM_REG];

LABEL_INFO labels[5];

int main(int argc, char **argv)
{
    char *sourceCode;

    for(short i = (LABEL_MAX - 1); i >= 0; i--){
        labels[i].labelName = NULL;
        for(short j = MAX_OFFSET - 1; j >= 0; j--){
            labels[i].offsets[j] = 0;
        }
    }

    //must malloc separately since we can't alloc more than 64kb in one go
    for(int i = 0; i < MEM_ROWS; i++){
        memory[i] = malloc(sizeof(char) * WORD_SIZE + 1);
        if(memory[i] == NULL)
            exit(1);
    }

    for(int i = 0; i < NUM_REG; i++) {
        regFile[i] = (char *)malloc(sizeof(char) * WORD_SIZE + 1);
        if(regFile[i] == NULL)
            exit(1);
    }

    init();

    strcpy(memory[HEAP_SEGMENT],     "00000000000000000000000000000010");
    strcpy(memory[HEAP_SEGMENT + 1], "00000000000000000000000000000110");
    strcpy(memory[HEAP_SEGMENT + 2], "00000000000000000000000000000111");
    strcpy(memory[HEAP_SEGMENT + 3], "00000000000000000000000000001001");

    strcpy(regFile[7], "00000000000000001111100111111111"); //SP
    strcpy(regFile[6], "00000000000000000000000000000000"); //RA
    strcpy(regFile[4], "00000000000000000000001010010101"); //starting array addr

    //UNCOMMENT FOR search even number elements - value exists
    strcpy(regFile[5], "00000000000000000000000000000100"); //length 4 - even number
    //strcpy(regFile[1], "00000000000000000000000000000111"); //exists - value we are looking for

    //UNCOMMENT FOR search even number elements - value doesn't exist
    //strcpy(regFile[5], "00000000000000000000000000000100"); //length 4 - even number
    //strcpy(regFile[1], "00000000000000000000000000000011"); //doesn't exists - value we are looking for

    //UNCOMMENT FOR search odd number elements - value exists
    //strcpy(memory[HEAP_SEGMENT + 4], "00000000000000000000000001000001");
    //strcpy(regFile[5], "00000000000000000000000000000101"); //length 5 - odd number
    //strcpy(regFile[1], "00000000000000000000000000000010"); //exists - value we are looking for

    //UNCOMMENT FOR search odd number elements - value doesn't exist
    //strcpy(memory[HEAP_SEGMENT + 4], "00000000000000000000000001000001");
    //strcpy(regFile[5], "00000000000000000000000000000101"); //length 5 - odd number
    //strcpy(regFile[1], "00000000000000000000000000000011"); //doesn't exists - value we are looking for

    EXEC_INFO info = initCPU(); //need to init the PC
    sourceCode = checkAndGetArg(argc, argv);
    loadAndStoreInstrs(sourceCode, &info);
    runProgram(info);

    //UNCOMMENT FOR BINARY SEARCH RESULT
    if(strcmp(regFile[1], regFile[2]) == 0) {
        printf("found the value %s at memory location %d\n", regFile[2], binaryToDecimal(regFile[3], WORD_SIZE));
    }else {
        printf("value was not in the array\n");
    }

    freeMemory();
    return 0;
}

//will get the file name from arg
char *checkAndGetArg(int argc, char **arg){
    char *file;

    if(argc > 2) {
        printf("Incorrect number of arguments. File is required\n");
        exit(1);
    }

    file = (char *)malloc(sizeof(char) * strlen(arg[1]) + 1);
    strcpy(file, arg[1]);
    return file;
}

void init() {

    flags[WORD_SIZE] = '\0';
    PC[PC_SIZE] = '\0';
    memAddr[WORD_SIZE] = '\0';
    memData[WORD_SIZE] = '\0';
    instrReg[WORD_SIZE] = '\0';

    for(unsigned int i = 0; i < WORD_SIZE; i++) {
        flags[i] = '0';
        memAddr[i] = '0';
        memData[i] = '0';
        instrReg[i] = '0';
    }

    for(unsigned int row = 0; row < MEM_ROWS; row++){
        for(unsigned int col = 0; col < WORD_SIZE; col++){
            memory[row][col] = '0';
        }
        memory[row][WORD_SIZE] = '\0';
    }

    for(unsigned int row = 0; row < NUM_REG; row++){
        for(unsigned int col = 0; col < WORD_SIZE; col++){
            regFile[row][col] = '0';
        }
        regFile[row][WORD_SIZE] = '\0';
    }

}

void freeMemory() {
    //free memory
    for(int i = 0; i < NUM_REG; i++){
        free(regFile[i]);
    }

    for(int i = 0; i < MEM_ROWS; i++) {
        free(memory[i]);
    }
}
