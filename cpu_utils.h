//
// Created by dearm on 9/17/2016.
// Utility functions for our CPU such as decoders, converters and the like
//

#ifndef DESIGN_ISA_CPU_UTILS_H
#define DESIGN_ISA_CPU_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "opcodes.h"

//mem split sizes
#define BOOT_SECTOR 512


#define WORD_SIZE 32
#define PC_SIZE 16
#define MEM_ROWS 64000 //number of rows for 16b addressing with 32b WORD 64KB total memory
#define NUM_REG 8

#define BOOT_ADDR 0

void initCPU(unsigned char *PC);
void loadAndStoreInstrs(char *fileName, unsigned char **memory);
void runProgram();
unsigned char *ALU(unsigned char *opLeft, unsigned char *opRight);

unsigned char *convertInstrToBin(unsigned char *instr);
unsigned char *addBinary(unsigned char *opLeft, unsigned char *opRight);

unsigned char *convertToBin(int toConvert, bool isImmVal);


#endif //DESIGN_ISA_CPU_UTILS_H
