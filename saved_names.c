#include "bool.h"
#include "saved_names.h"
#include <string.h>

/**
 * Check if a word is a saved name.
 * A saved name can be a macro instruction, directive, or operation.
 */
boolean is_saved_name(char *word)
{
	return is_mcro_inst(word) || is_directive(word) || is_two_op_inst(word) || is_one_op_inst(word) || is_zero_op_inst(word);
}

/**
 * Check if a word is a macro instruction.
 */
boolean is_mcro_inst(char *word)
{
	int i;
	const char *mcr[] = {"mcro", "endmcro"};
	for(i=0; i<2; i++)
		if(!strcmp(mcr[i], word))
			return TRUE;
	
	return FALSE;
}

/**
 * Check if a word is a directive.
 */
boolean is_directive(char *word)
{
	int i;
	const char *dirs[] = {".data", ".string", ".extern", ".entry"};
	for(i=0; i<4; i++)
		if(!strcmp(dirs[i], word))
			return TRUE;
	
	return FALSE;
}

/**
 * Check if a word is a two-operand instruction.
 */
boolean is_two_op_inst(char *word)
{
	int i;
	const char *groupA[] = {"mov", "cmp", "add", "sub", "lea"};
	for(i=0; i<5; i++)
		if(!strcmp(groupA[i], word))
			return TRUE;
	
	return FALSE;
}

/**
 * Check if a word is a one-operand instruction.
 */
boolean is_one_op_inst(char *word)
{
	int i;
	const char *groupB[] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
	
	for(i=0; i<9; i++)
		if(!strcmp(groupB[i], word))
			return TRUE;
	
	return FALSE;
}

/**
 * Check if a word is a zero-operand instruction.
 */
boolean is_zero_op_inst(char *word)
{
	int i;
	const char *groupC[] = {"rts", "stop"};

	for(i=0; i<2; i++)
		if(!strcmp(groupC[i], word))
			return TRUE;
	
	return FALSE;
}
