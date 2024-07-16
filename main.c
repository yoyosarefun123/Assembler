#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"

int main(int argc, char *argv[])
{
	int i = 1;
	/*print welcome message and explanation if someone just called the assembler with no input files*/
	if (argc <= 1) {
		printf("Welcome to our assembler!\nThis program takes assembly code (from an assembly language invented in the maabada university course)\nand assembles it, turning it into a object file type that the computer is capable of reading (in theory)!\n\nTo use, write in the console: ./assembler {file_name_1} ... {file_name_n}\n");
	}
	/*goes through the input files and runs the assembler on each one seperately*/
	for (; i < argc; i++) {
		assembler(argv[i]);
	}
	return 0;
}

