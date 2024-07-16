#ifndef PARSERING_LINES_H
#define PARSERING_LINES_H
#include "bool.h"
#include "ast.h"

#define MAX_INT_10BITS 511
#define MIN_INT_10BITS -512
#define MAX_INT_12BITS 2047
#define MIN_INT_12BITS -2048


/**
this function skips the white chars in a line and moves the index to the first non white char
*/
void skipWhites(char *input, int *currIndex);

/**
this fonction gets the first word in the input line after the index i to the word "currStr"
*/
void getWord(char *input, char **currStr, int *i);

/**
this function returns a pointer to a string that is the name+file_type together
*/
char* add_file_type(char *name, char *file_type);

/**
this function checks if the next char is a comma or not, and advances the index respectively
*/
boolean is_there_comma(char *line, int *index);

/**
this function returns the next non white character and advances the index respectively
*/
char next_char(char *str, int i);

/**
this function gets the first number starting after an index in a line
it also finds error if there are any
*/
int getNum(char *line, int *curr_data, int *index, int is_purpose_data, astNode *line_tree);
#endif
