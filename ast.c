#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ast.h"
/*#include "linked_list.h"
#include "dictionary.h"*/
#include "saved_names.h"
#include "parsering_lines.h"
#include "bool.h"


#define MAX_LABEL_LEN 31
#define MAX_LABELS_IN_DIR 30
#define MAX_LINE_LEN 81
#define MAX_ERROR_LEN 200
#define MAX_STRING_LEN 81
#define MAX_NUMBERS_LEN 30
#define NULL_TERMINATOR 1
#define SRC 0
#define TRG 1



typedef enum { /*this enum is used later, it determines the type of an operand's address*/
	IMMED = 1,
	LABEL = 3,
	REG = 5
} operand_address;

union operand_content{ /*this union is used later and contains the content of an instruction operand*/
	int immed;
	char label[MAX_LABEL_LEN];
	int reg;
};

struct astNode {
	char line_label[MAX_LABEL_LEN];
	char line_error[MAX_ERROR_LEN];
	boolean labeled;
	
	enum { /*this enum determines the type of the line*/
		ERR_LINE = -1,
		INST_LINE,
		DIR_LINE,
		EMPTY_LINE
		} line_type;
	
	union { /*this union contains the content of the line*/
		
		struct { /*this is the struct that contains the content of the line if it is a directive line*/
			enum { /*this enum determines what is the type of the directive*/
				DATA,
				STR,
				ENT,
				EXT
			} directive_type;
			
			union { /*this union contains the content of the directive*/
				struct { /*in case of .data directive, this struct contains the numbers array and also a counter of how many numers are*/
					int num_array[MAX_NUMBERS_LEN];
					int num_cnt;
					} data;
				char string[MAX_STRING_LEN + NULL_TERMINATOR]; /*in case of .string, this char[] contains the string*/
				struct { /*in case of .extern or .entry this struct contains an array of char[]s - the labels, and a counter of how many labels are*/
					char labels_array[MAX_LABELS_IN_DIR][MAX_LABEL_LEN];
					int labels_cnt;
					} ext_ent;
			} directive_content;
		} directive;
		
		struct { /*this struct contains the content of an instruction line*/
			enum { /*this enum determines the type of the instruction*/
				/*2 OPERAND INSTRUCTIONS (except lea)*/
				MOV_INST,
				CMP_INST,
				ADD_INST,
				SUB_INST,
				/*1 OPERAND INSTRUCTIONS*/
				NOT_INST,
				CLR_INST,
				/*lea which is two operand*/
				LEA_INST,
				INC_INST,
				DEC_INST,
				JMP_INST,
				BNE_INST,
				RED_INST,
				PRN_INST,
				JSR_INST,
				/*0 OPERANDS INSTRUCTIONS*/
				RTS_INST,
				STOP_INST
			} inst_opcode;	
		
			union { /*this union contains the content of the instruction*/
				struct { /*in case of a 2 operand instruction, an array of 2 enums (operand's address type) and an array of 2 unions (operand's content*/
					operand_address oa[2];
					union operand_content oc[2];
				} two_operands_inst;
				struct { /*in case of a 1 operand instruction, only one of these enum and union will be created*/
					operand_address oa;
					union operand_content oc;
				} one_operand_inst;
			} operands;
		} instruction;
	} line_content;
};

struct astNode* create_new_ast_node()
{
	struct astNode *new_node = (struct astNode*)malloc(sizeof(struct astNode));
	if(!new_node) 
	{
		printf("Memory allocation failed. Sorry.");
		exit(1);
	}
	/*sets everything to default*/
	new_node->line_label[0] = '\0';
	new_node->line_error[0] = '\0';
	new_node->labeled = FALSE;
	new_node->line_type = EMPTY_LINE;
	new_node->line_content.directive.directive_type = DATA;
	new_node->line_content.directive.directive_content.data.num_array[0] = 0;
	new_node->line_content.directive.directive_content.data.num_cnt = 0;
	new_node->line_content.directive.directive_content.string[0] = '\0';
	new_node->line_content.directive.directive_content.ext_ent.labels_array[0][0] = '\0';
	new_node->line_content.directive.directive_content.ext_ent.labels_cnt = 0;
	new_node->line_content.instruction.inst_opcode = MOV_INST;
	new_node->line_content.instruction.operands.two_operands_inst.oa[SRC] = IMMED;
	new_node->line_content.instruction.operands.two_operands_inst.oa[TRG] = IMMED;
	new_node->line_content.instruction.operands.two_operands_inst.oc[SRC].immed = 0;
	new_node->line_content.instruction.operands.two_operands_inst.oc[SRC].label[0] = '\0';
	new_node->line_content.instruction.operands.two_operands_inst.oc[SRC].reg = 0;
	new_node->line_content.instruction.operands.two_operands_inst.oc[TRG].immed = 0;
	new_node->line_content.instruction.operands.two_operands_inst.oc[TRG].label[0] = '\0';
	new_node->line_content.instruction.operands.two_operands_inst.oc[TRG].reg = 0;
	new_node->line_content.instruction.operands.one_operand_inst.oa = IMMED;
	new_node->line_content.instruction.operands.one_operand_inst.oc.immed = 0;
	new_node->line_content.instruction.operands.one_operand_inst.oc.label[0] = '\0';
	new_node->line_content.instruction.operands.one_operand_inst.oc.reg = 0;
	
	return new_node;
}


int ast_get_line_type(struct astNode *tree) {
	return tree->line_type;
}

char* ast_get_line_error(struct astNode *tree) {
	int len;
	char *error;
	len = strlen(tree->line_error);
	if(len!=0)
	{
		error = (char*)malloc(sizeof(char) * (len + NULL_TERMINATOR)); /*creates a copy of the string to keep the data structure encapsulated*/
		if(!error)
		{
			printf("Couldn't allocate memory. Sorry\n");
			exit(1);
	
		}
		strcpy(error, tree->line_error);
		error[len] = '\0';
		return error;
	}
	return NULL;
}

void ast_set_line_error(char *str, astNode *tree)
{
	if (tree->line_type != ERR_LINE) {
		strcpy(tree->line_error, str);
		tree->line_type = ERR_LINE;
	}
}

boolean ast_is_label(astNode *tree)
{
	return tree->labeled;
}

char* ast_get_line_label(struct astNode *tree) {
	int len;
	char *label;
	
	len = strlen(tree->line_label);
	if(len!=0)
	{
		label = (char*)malloc(sizeof(char) * (len + NULL_TERMINATOR));
		if(!label)
		{
			printf("Couldn't allocate memory. Sorry\n");
			exit(1);
	
		}
		strcpy(label, tree->line_label);
		label[len] = '\0';
		return label;
	}
	return NULL;
}

int ast_dir_get_type(struct astNode *tree) {
	return tree->line_content.directive.directive_type;
}

int* ast_dir_data_get_num_array(struct astNode *tree) {
	int i, len;
	int *array;
	
	len = tree->line_content.directive.directive_content.data.num_cnt;
	if(len!=0)
	{
		array = (int*)malloc(sizeof(int) * len);
	
		if(!array)
		{
			printf("Couldn't allocate memory. Sorry\n");
			exit(1);
	
		}
		for(i=0; i<len; i++)
		{
			array[i] = tree->line_content.directive.directive_content.data.num_array[i];
		}
		return array;
	}
	return NULL;
}

int ast_dir_data_get_num_cnt(struct astNode *tree) {
	return tree->line_content.directive.directive_content.data.num_cnt;
}

char* ast_dir_get_string(struct astNode *tree) {
	int len;
	char *str;
	
	len = strlen(tree->line_content.directive.directive_content.string);
	str = (char*)malloc(sizeof(char) * (len + NULL_TERMINATOR));
	if(!str)
	{
		printf("Couldn't allocate memory. Sorry\n");
		exit(1);
	
	}
	strcpy(str, tree->line_content.directive.directive_content.string);
	str[len] = '\0';
	return str;
}
char** ast_dir_get_labels(struct astNode *tree) {
	int len, labels_cnt, i;
	char **labels;
	
	labels_cnt = tree->line_content.directive.directive_content.ext_ent.labels_cnt;
	if(labels_cnt!=0)
	{
		labels = (char**)malloc(sizeof(char*) * labels_cnt);
		if(!labels)
		{
			printf("Couldn't allocate memory. Sorry\n");
			exit(1);
	
		}
		
		for(i=0; i<labels_cnt; i++)
		{
			len = strlen(tree->line_content.directive.directive_content.ext_ent.labels_array[i]);
			labels[i] = (char*)malloc(sizeof(char) * (len + NULL_TERMINATOR));
			if(!labels[i])
			{
				printf("Couldn't allocate memory. Sorry\n");
				exit(1);
	
			}
			strcpy(labels[i], tree->line_content.directive.directive_content.ext_ent.labels_array[i]);
			labels[i][len] = '\0';
		}	
		return labels;
	}
	return NULL;
	
}

int ast_dir_get_label_cnt(struct astNode *tree) {
	return tree->line_content.directive.directive_content.ext_ent.labels_cnt;
}

int ast_inst_get_opcode(struct astNode *tree) {
	return tree->line_content.instruction.inst_opcode;
}
int ast_inst_oneop_get_addr(struct astNode *tree) {
	return tree->line_content.instruction.operands.one_operand_inst.oa;
}

int ast_inst_oneop_get_immed(struct astNode *tree)
{
	return tree->line_content.instruction.operands.one_operand_inst.oc.immed;
}
char* ast_inst_oneop_get_label(struct astNode *tree) {
	int len;
	char *label;
	
	len = strlen(tree->line_content.instruction.operands.one_operand_inst.oc.label);
	label = (char*)malloc(sizeof(char) * (len + NULL_TERMINATOR));
	if(!label)
	{
		printf("Couldn't allocate memory. Sorry\n");
		exit(1);
	
	}
	strcpy(label, tree->line_content.instruction.operands.one_operand_inst.oc.label);
	label[len] = '\0';
	return label;
}
int ast_inst_oneop_get_reg(struct astNode *tree) {
	return tree->line_content.instruction.operands.one_operand_inst.oc.reg;
}

int ast_inst_twoop_src_get_addr(astNode *tree)
{
	return (tree->line_content.instruction.operands.two_operands_inst.oa)[SRC];
}
int ast_inst_twoop_trg_get_addr(astNode *tree) {
	return (tree->line_content.instruction.operands.two_operands_inst.oa)[TRG];
}
int ast_inst_twoop_src_get_immed(astNode *tree) {
	return (tree->line_content.instruction.operands.two_operands_inst.oc)[SRC].immed;
}
int ast_inst_twoop_trg_get_immed(astNode *tree) {
	return (tree->line_content.instruction.operands.two_operands_inst.oc)[TRG].immed;
}
char* ast_inst_twoop_src_get_label(astNode *tree) {
	int len;
	char *label;
	
	len = strlen((tree->line_content.instruction.operands.two_operands_inst.oc)[SRC].label);
	label = (char*)malloc(sizeof(char) * (len + NULL_TERMINATOR));
	if(!label)
	{
		printf("Couldn't allocate memory. Sorry\n");
		exit(1);
	
	}
	strcpy(label, (tree->line_content.instruction.operands.two_operands_inst.oc)[SRC].label);
	return label;
}
char* ast_inst_twoop_trg_get_label(astNode *tree) {
	int len;
	char *label;
	
	len = strlen((tree->line_content.instruction.operands.two_operands_inst.oc)[TRG].label);
	label = (char*)malloc(sizeof(char) * (len + NULL_TERMINATOR));
	if(!label)
	{
		printf("Couldn't allocate memory. Sorry\n");
		exit(1);
	
	}
	strcpy(label, (tree->line_content.instruction.operands.two_operands_inst.oc)[TRG].label);
	return label;
}
int ast_inst_twoop_src_get_reg(astNode *tree) {
	return (tree->line_content.instruction.operands.two_operands_inst.oc)[SRC].reg;
}
int ast_inst_twoop_trg_get_reg(astNode *tree) {
	return (tree->line_content.instruction.operands.two_operands_inst.oc)[TRG].reg;
}



/******************************************************************build_ast_from_line****************************************************************************/

/**
this function checks if a line has a label in the start of it
*/
boolean check_for_label(char *line, char **currWord, int *index, int *index_backup, struct astNode *line_tree);

/**
checks if the word passed to it is a valid label.
if it is, returns TRUE.
if it isnt, sets the astNode's error to a specific error and returns FALSE.
*/
boolean is_legal_label(char *word, struct astNode *line_tree);

/**
acts like strchr from string.h but returns the index instead of a pointer
*/
int my_strchr(char *str, char c);

/**
this function creates an ast from a directive line
*/
void create_directive_ast(astNode *line_tree, char **currWord, char *line, int *index);

/**
this function gets a .data directive line and creates an ast from it
*/
void create_directive_data_ast(astNode *line_tree, char **currWord, char *line, int *index);

/**
this function checks the string after .string directive
it sets the ast error to be something if any errors are detected
*/
void get_string(char *line, int *index, astNode *line_tree);

/**
this function creates an ast from an .extern or .entry directive line
*/
void create_directive_ent_ext_ast(astNode *line_tree, char **currWord, char *line, int *index);

/**
checks if the word passed to it is a valid number.
if it is, it returns TRUE.
if it isn't, it sets the astNode's error to a specific error and returns FALSE.
*/
boolean is_valid_num(char *word, struct astNode *line_tree);

/**
this function checks if the last visible character of the line is "
*/
boolean is_apost_last(char *line);

/**
returns TRUE if the character is an operation (+ - / *)
*/
boolean isoperation(char c);

/**
returns TRUE if the word passed to it is a number, FALSE otherwise
*/
boolean is_num(char *word);

/**
returns TRUE if the word passed to it is a potential label
*/
boolean is_valid_label(char *str);

/**
checks if the word passed to it is a valid register name.
if it is, returns TRUE.
if it isnt, sets the astNode's error to a specific error and returns FALSE.
*/
boolean is_reg(char *word, astNode *line_tree);

/**
this function gets an instruction line and creates an ast from it
*/
boolean create_instruction_ast(astNode *line_tree, char **currWord, char *line, int *index);

/**
this function gets a two operand instruction line and creates from it an ast
*/ 
boolean create_instruction_twoop_ast(astNode *line_tree, char **currWord, char *line, int *index);

/**
this function gets a one operand instruction line and creates an ast from it
*/
boolean create_instruction_oneop_ast(astNode *line_tree, char **currWord, char *line, int *index);

/**
returns how many operands a specific instruction needs
*/
int how_many_operands_need(char *currWord);

/**
returns TRUE if the word is an instruction, otherwise FALSE
*/
boolean is_inst(char *currWord);

/**
sets the astNode's inst_opcode depends on the instruction given
*/
void set_inst_type(char *inst, struct astNode *line_tree);




struct astNode* build_ast_from_line(char *line)
{
	char *currWord;
	int index, index_backup; /*an index and a backup - the backup is used to determine if we got -1 from getWord because of a comma or because of white space*/
	struct astNode *line_tree = create_new_ast_node();
	if(!line_tree)
	{
		printf("Couldn't allocate memory. Sorry\n");
		exit(1);
	
	}
	
	line_tree->labeled = FALSE; /*this is the default, will potentially be changed later*/
	
	index = 0;
	
	
	currWord = (char*)malloc(MAX_LINE_LEN * sizeof(char));
	if(!currWord)
	{
		printf("Couldn't allocate memory. Sorry\n");
		exit(1);
	
	}
	
	index_backup = index;
	getWord(line, &currWord, &index);
	
	if (index == -1) /*if detWord didnt get any word to currWord, it sets index to -1, in that case it could be because a white space or because a comma*/
	{
		if(is_there_comma(line, &index_backup)) /*if getWord stopped because of a comma, we would like to return an error*/
		{
			ast_set_line_error("Illegal comma", line_tree);
		}
		else 
			line_tree->line_type = EMPTY_LINE;
		free(currWord);
		return line_tree;
	}
	/*printf("checkpoint 1\n");*/
	if(check_for_label(line, &currWord, &index, &index_backup, line_tree)) /*check if the current line comes with a label in its ceginning*/
	{
		free(currWord);
		return line_tree;
	}
	
	
	
	if(is_directive(currWord))
	{
		create_directive_ast(line_tree, &currWord, line, &index);
		free(currWord);
		return line_tree;
	}
	
	else if(is_inst(currWord))
	{
		if(create_instruction_ast(line_tree, &currWord, line, &index)) /*if there were any errors in the making of this ast, the function return TRUE and enter  this condition*/
		{
			free(currWord);
			return line_tree;
		}
	}
	else
	{
		ast_set_line_error("Undefined command", line_tree);
		free(currWord);
		return line_tree;
	}
	
	index_backup = index;
	getWord(line, &currWord, &index);
	if(index == -1)
	{
		if(is_there_comma(line, &index_backup)) /*if we got -1 from getWord, check if there is a comma that the getWord missed*/
		{
			ast_set_line_error("Extraneous text after end of command", line_tree);
		}
	}
	else /*in this case, getWord found somthing after the command*/
	{
		ast_set_line_error("Extraneous text after end of command", line_tree);
	}
	
	free(currWord);
	return line_tree;
}

boolean check_for_label(char *line, char **currWord, int *index, int *index_backup, struct astNode *line_tree)
{
	boolean is_label = FALSE;
	
	if(my_strchr(*currWord, ':') != -1) /*if there is a : in the first word*/
	{
		*index = 0;
		skipWhites(line, index);
		*index += my_strchr(*currWord, ':') + 1; /*this is the first word and we add to it the much of white space before, we set the index to this so we can check for things right after the label*/
		(*currWord)[my_strchr(*currWord, ':')] = '\0'; /*sets the : to '\0' so we can check if the string before it is a valid label*/
		if(is_legal_label(*currWord, line_tree)) /*if the label is legal we set the ast's label to be it*/
		{
			strcpy(line_tree->line_label, *currWord);
			is_label=TRUE;
			line_tree->labeled = TRUE;
		}
		else /*otherwise the error is already in the ast*/
		{
			return TRUE;
		}
	}
	
	if(next_char(line, *index) == ':') /*if the next character in the line, after the : of the label, is another :, there is double : or if there wasn't a : at first, the space between the label itself to the : is too big*/
	{
		if(is_label)
		{
			ast_set_line_error("Illegal label: Too many colons in a row", line_tree);
			return TRUE;
		}
		else
		{
			ast_set_line_error("Illegal label: Too much space between label and colon", line_tree);
			return TRUE;
		}
	}	
	
	if(is_label) /*if there was a label, we want our index to be after this label*/
	{
		*index = my_strchr(line, ':') + 1;
		*index_backup = *index;
		getWord(line, &(*currWord), index);
	}
		
	if (*index == -1) /*if we didnt receive a word. is it because of , or not*/
	{
		if(is_there_comma(line, index_backup))
			ast_set_line_error("Illegal comma", line_tree);
		else
			ast_set_line_error("Label without command", line_tree);
		return TRUE;
	}
	
	return FALSE;
}

boolean is_legal_label(char *word, struct astNode *line_tree)
{
	int i;
	if(strlen(word)>31) /*the label supposed to be 31 characters or less*/
	{
		ast_set_line_error("Illegal label: Label is too long (max length is 31)", line_tree);
		return FALSE;
	}
	if(is_saved_name(word)) /*label cannot be a saved name*/
	{
		ast_set_line_error("Illegal label: Label cannot be a saved name", line_tree);
		return FALSE;
	}
	if(strlen(word)<1) /*label cannot be empty*/
	{
		ast_set_line_error("Illegal label: Missing label name", line_tree);
		return FALSE;
	}
	if(!isalpha(word[0])) /*first label's letter must be alphabetic*/
	{
		ast_set_line_error("Illegal label: First character cannot be non-alphabetic", line_tree);
		return FALSE;
	}
	else
	{
		for(i=1; i<strlen(word); i++)
			if(!isalnum(word[i])) /*all other label's letters must be alphanumeric*/
			{
				ast_set_line_error("Illegal label: Label cannot contain non-alphanumeric cahracters", line_tree);
				return FALSE;
			}
	}
	return TRUE;
}

int my_strchr(char *str, char c)
{
	int i = 0;
	for(;i<strlen(str); i++)
		if(str[i]==c)
			return i;
	return -1;
}

void create_directive_ast(astNode *line_tree, char **currWord, char *line, int *index)
{
	
	line_tree->line_type = DIR_LINE;
	if(!strcmp(*currWord, ".data"))
	{
		create_directive_data_ast(line_tree, currWord, line, index);
	}
	else if (!strcmp(*currWord, ".string"))
	{
		line_tree->line_content.directive.directive_type = STR;
		get_string(line, index, line_tree);
	}
	else if (!strcmp(*currWord, ".entry") || !strcmp(*currWord, ".extern"))
	{
		create_directive_ent_ext_ast(line_tree, currWord, line, index);
	}
}

void create_directive_data_ast(astNode *line_tree, char **currWord, char *line, int *index)
{
	int curr_data;
	boolean is_consec = TRUE;
	
	line_tree->line_content.directive.directive_type = DATA;
	line_tree->line_content.directive.directive_content.data.num_cnt = 0;
	
	while(is_consec) /*while the numbers are consecutive, if is_consec is false, it means there is no comma - so the numbers arent consecutive now*/
	{
		if(getNum(line, &curr_data, index, TRUE, line_tree)) /*getNum returns 1 if got errors, so will enter the condition*/
		{
			return;
		}
			
		line_tree->line_content.directive.directive_content.data.num_array[line_tree->line_content.directive.directive_content.data.num_cnt] = curr_data;
		(line_tree->line_content.directive.directive_content.data.num_cnt)++;
		
		if(!is_there_comma(line, index)) /*if there aint no comma*/
		{
			getWord(line, currWord, index);
			if(*index==-1) /*if it was the last number, nothing is wrong*/
			{
				return;
			}
			else if(is_num(*currWord)) /*if the next token is a number, we miss a comma*/
			{
				ast_set_line_error("Missing comma", line_tree);
				return;
			}
			else
			{
				ast_set_line_error("Extreneous text after end of command", line_tree);
				return;
			}
			is_consec = FALSE; /*the numbers arent consecutive anymore because there is no comma*/
		}
	}
}

void get_string(char *line, int *index, astNode *line_tree)
{
	int first_apost, second_apost, i;
	
	skipWhites(line, index);
	if(*index==-1) /*if there is no string*/
	{
		ast_set_line_error("Missing string after .string directive", line_tree);
		return;
	}
	
	first_apost = my_strchr(line, '\"'); /*finds the first " */
	if(first_apost == -1) /*my_strchr returns -1 if the char was not found*/
	{
		ast_set_line_error("Missing apostrophes in string after '.string' directive", line_tree);
		return;
	}

	second_apost = first_apost + my_strchr(line + first_apost + 1, '\"') + 1; /*the index of the first " + the index of the first " after first_apost + 1 that is the " itself*/

	if(second_apost == first_apost) /*in other words, if my_strchr in the line above returned -1*/
	{
		if(line[*index] == '\"') /*if the first char after .string was ", the closing " are missing*/
		{
			ast_set_line_error("Missing closing apostrophes in .string directive", line_tree);
			return;
		}

		if(is_apost_last(line)) /*if " is the last char here, the opening " are missing*/
		{
			ast_set_line_error("Missing opening apostrophes in .string directive", line_tree);
			return;
		}
		ast_set_line_error("Illegal string", line_tree);
		return;
	}
	/*now we got two or more apostrophes*/
	if(line[*index] != '\"') /*if the first char is not ", there is something before the string*/
	{
		ast_set_line_error("Extraneous text before string", line_tree);
		return;
	}
	
	for(i = first_apost+1; i<second_apost; i++) /*check if the chars inside the apostrophes are printable*/
	{
		if(!isprint(line[i]))
		{
			ast_set_line_error("String cannot contain non-printable characters", line_tree);
			return;
		}
	}
	/*line+first_apost+1 is the first char after the first " (+first_apost reaches the ", +1 for the next char. second_apost-first__apost-1 is the length of the string inside the apostrophes, excluding the "s themselves of course*/
	strncpy(line_tree->line_content.directive.directive_content.string, line + first_apost + 1, second_apost - first_apost - 1); /*copy string into ast*/
	line_tree->line_content.directive.directive_content.string[second_apost - first_apost - 1] = '\0'; /*add null terminator*/
	line_tree->line_content.directive.directive_type = STR;
	
	second_apost++; /*now second_apost will be used for a different prupose - to check if the charcters after the string are white or not*/
	skipWhites(line, &second_apost); /*skip the white chars after the second " */

	if(line[second_apost] != '\0' && !isspace(line[second_apost]))
	{
		ast_set_line_error("Extraneous text after end of string", line_tree);
		return;
	}
	return;
}
	
void create_directive_ent_ext_ast(astNode *line_tree, char **currWord, char *line, int *index)
{
	int index_backup;
	boolean is_consec, is_first_label;
	is_consec = TRUE;
	is_first_label = TRUE;
	
	if (!strcmp(*currWord, ".entry")) line_tree->line_content.directive.directive_type = ENT;
	else line_tree->line_content.directive.directive_type = EXT;
		
	line_tree->line_content.directive.directive_content.ext_ent.labels_cnt = 0;
		
	while(is_consec)
	{	
		index_backup = *index;
		getWord(line, currWord, index);
		if(*index == -1)
		{
			if(is_there_comma(line, &index_backup))
				ast_set_line_error("Illegal comma", line_tree);
			else if(is_first_label) /*if it's the first label, there are no labels*/
				ast_set_line_error("Missing label after .extern/.entry directive", line_tree);
			else
				ast_set_line_error("Extraneous text after end of command", line_tree);
			return;
		}
		if(is_legal_label(*currWord, line_tree))
		{
			strcpy(line_tree->line_content.directive.directive_content.ext_ent.labels_array[line_tree->line_content.directive.directive_content.ext_ent.labels_cnt], *currWord); 
			(line_tree->line_content.directive.directive_content.ext_ent.labels_cnt)++;
		}
		else /*if the label is not a label, the error is in the ast thanks to is_legal_label, so we return it*/
		{
			return;
		}
		if(!is_there_comma(line, index)) /*like the create .data ast function*/
		{
			getWord(line, currWord, index);
			if(*index == -1) 
			{
				return;
			}
			else if(is_valid_label(*currWord))
			{
				ast_set_line_error("Missing comma", line_tree);
				return;
			}
			else
			{
				ast_set_line_error("Extreneous text after end of command", line_tree);
				return;
			}
			is_consec = FALSE;
		}
		is_first_label = FALSE; /*after the first iteration the label is no more first*/
	}

	return;
}

boolean is_valid_num(char *word, struct astNode *line_tree)
{
	int i = 0;
	if(isalpha(word[i])) /*onlyt return FALSE and not an error because it might be a label*/
		return FALSE;
	if(!isdigit(word[i]) && word[i]!='+' && word[i]!='-') /*only return FALSE, it will return an error in the label checking stage because it is more likely that the programmer meant a label and not a number*/
		return FALSE;
	for(i = 1; i<strlen(word); i++)
		if(!isdigit(word[i]))
		{
			if(word[i] == '.')
			{
				ast_set_line_error("Illegal nunmber: The machine does not support floating point", line_tree);
			}
			if(isoperation(word[i]))
			{
				ast_set_line_error("Illegal number: The command cannot handle arithmetic expressions", line_tree);
			}
			return FALSE;
		}
	return TRUE;

}

boolean isoperation(char c)
{
	return c=='+' || c=='-' || c=='/' || c=='*';
}

boolean is_apost_last(char *line)
{
	int i;
	boolean is_last_char_apost = FALSE;
	for(i = 0; line[i] != '\0'; i++)
	{
		if(line[i] == '\"')
			is_last_char_apost = TRUE;
		else if(!isspace(line[i]))
			is_last_char_apost = FALSE;
	}
	
	return is_last_char_apost;
}

boolean is_num(char *word)
{
	int i;
	for(i = 0; i<strlen(word); i++)
		if(!isdigit(word[i])) /*all chars must be digits*/
			return FALSE;
	return TRUE;
}

boolean is_valid_label(char *str)
{
	int i = 0;
	if(strlen(str)<32) /*must be 31 chars or less*/
	{
		if(isalpha(str[i])) /*first char must be alphabetic*/
		{
			for(i = 1; i<strlen(str); i++)
			{
				if(!isalnum(str[i])) /*other chars must be alphanumeric*/
					return FALSE;
			}
		}
		else return FALSE;
	}
	return TRUE;
	
}

boolean is_reg(char *word, astNode *line_tree)
{
	if(strlen(word)==1)
	{
		if(word[0]=='@') /*if the word starts with @, it must be a call for an operand*/
		{
			ast_set_line_error("Missing register name", line_tree);
			return FALSE;
		}
		return FALSE;
	}
	if(strlen(word)==2)
	{
		if(word[0]=='@')
		{
			if(word[1]=='r')
			{
				ast_set_line_error("Missing register number", line_tree);
				return FALSE;
			}
			ast_set_line_error("Illegal register name", line_tree);
			return FALSE;
		}
		return FALSE;
	}
	else
	{
		if(word[0]=='@')
		{
			if(word[1]=='r')
			{
				if(word[2]>='0' && word[2]<='7' && strlen(word)==3)
				{
					return TRUE;
				}
				ast_set_line_error("Illegal register number", line_tree);
				return FALSE;
			}
			ast_set_line_error("Illegal register name", line_tree);
			return FALSE;
		}
		return FALSE;
	}
}

boolean create_instruction_ast(astNode *line_tree, char **currWord, char *line, int *index)
{
	line_tree->line_type = INST_LINE;
	switch(how_many_operands_need(*currWord))
	{
		case 0:
			set_inst_type(*currWord, line_tree);
			/*there are no operands to check, only the extraneous text is a possibility and it is being checked outside*/	
			break;
			
		case 1:
			set_inst_type(*currWord, line_tree);
			
			if(create_instruction_oneop_ast(line_tree, currWord, line, index)) /*if there were any errors in the process, we will return the ast right away. if there were'nt, we will check for extraneous text after the command*/
				return TRUE;
			
			break;
			
		case 2:
			set_inst_type(*currWord, line_tree);
				
			if(create_instruction_twoop_ast(line_tree, currWord, line, index))
				return TRUE;
		
			break;
			
		default: ast_set_line_error("Illegal number of operands", line_tree);
			 return TRUE;
	}
	return FALSE;
}

boolean create_instruction_oneop_ast(astNode *line_tree, char **currWord, char *line, int *index)
{
	int index_backup = *index;
	getWord(line, currWord, index);
	if (*index == -1)
	{
		if(is_there_comma(line, &index_backup))
			ast_set_line_error("Illegal comma", line_tree);
		else
		ast_set_line_error("Missing parameter", line_tree);
		return TRUE;
	}
	if(is_reg(*currWord, line_tree)) /*if the operand is a register*/
	{
		line_tree->line_content.instruction.operands.one_operand_inst.oa = REG;
		line_tree->line_content.instruction.operands.one_operand_inst.oc.reg = (*currWord)[2] - '0'; /*subtract the ASCII code of 0 from the num after @r*/
	}	
	else if(is_valid_num(*currWord, line_tree)) /*if the operand is a number*/
	{	
		index_backup = 0;
		line_tree->line_content.instruction.operands.one_operand_inst.oa = IMMED;
		if(getNum(*currWord, &(line_tree->line_content.instruction.operands.one_operand_inst.oc.immed), &index_backup, FALSE, line_tree)) /*if getNum found any errors in the number it will return TRUE and we will get into this condition*/
		{
			return TRUE;
		}
	}				
	else if(is_legal_label(*currWord, line_tree)) /*if the operand is a label*/
	{
		line_tree->line_content.instruction.operands.one_operand_inst.oa = LABEL;
		strcpy(line_tree->line_content.instruction.operands.one_operand_inst.oc.label, *currWord);
	}
	else
	{
		if(line_tree->line_type != ERR_LINE)
		{
			ast_set_line_error("Illegal operand", line_tree);
		}
		return TRUE;
	}
	return FALSE;
}

boolean create_instruction_twoop_ast(astNode *line_tree, char **currWord, char *line, int *index)
{
	int index_backup = *index;
	getWord(line, currWord, index);
				
	if(*index==-1)
	{
		if(is_there_comma(line, &index_backup))
			ast_set_line_error("Illegal comma",line_tree);
		else 
			ast_set_line_error("Missing parameter", line_tree);
		return TRUE;
	}
	
	if(is_reg(*currWord, line_tree))
	{
		line_tree->line_content.instruction.operands.two_operands_inst.oa[SRC] = REG;
		line_tree->line_content.instruction.operands.two_operands_inst.oc[SRC].reg = (*currWord)[2] - '0';
	}	
	else if(is_valid_num(*currWord, line_tree))
	{
		index_backup = 0;
		line_tree->line_content.instruction.operands.two_operands_inst.oa[SRC] = IMMED;
		if(getNum(*currWord, &(line_tree->line_content.instruction.operands.two_operands_inst.oc[SRC].immed), &index_backup, FALSE, line_tree))
		{
			return TRUE;
		}
	}				
	else  if(is_legal_label(*currWord, line_tree))
	{
		line_tree->line_content.instruction.operands.two_operands_inst.oa[SRC] = LABEL;
		strcpy(line_tree->line_content.instruction.operands.two_operands_inst.oc[SRC].label, *currWord);
	}
	else
	{
		if(line_tree->line_type != ERR_LINE)
		{
			ast_set_line_error("Illegal operand", line_tree);
		}
		return TRUE;
	}
				
	if(!is_there_comma(line, index))
	{
		ast_set_line_error("Missing comma", line_tree);
		return TRUE;
	}
	
	index_backup = *index;
	getWord(line, currWord, index);
				
	if (*index == -1)
	{
		if(is_there_comma(line, &index_backup))
			ast_set_line_error("Illegal double comma", line_tree);
		else 
			ast_set_line_error("Missing parameter", line_tree);
		return TRUE;
	}
				
	if(is_reg(*currWord, line_tree))
	{
		line_tree->line_content.instruction.operands.two_operands_inst.oa[TRG] = REG;
		line_tree->line_content.instruction.operands.two_operands_inst.oc[TRG].reg = (*currWord)[2] - '0';
	}	
	else if(is_valid_num(*currWord, line_tree))
	{
		index_backup = 0;
		line_tree->line_content.instruction.operands.two_operands_inst.oa[TRG] = IMMED;
		if(getNum(*currWord, &(line_tree->line_content.instruction.operands.two_operands_inst.oc[TRG].immed), &index_backup, FALSE, line_tree))
		{
			return TRUE;
		}
	}			
	else if(is_legal_label(*currWord, line_tree))
	{
		line_tree->line_content.instruction.operands.two_operands_inst.oa[TRG] = LABEL;
		strcpy(line_tree->line_content.instruction.operands.two_operands_inst.oc[TRG].label, *currWord);
	}
	else
	{
		if(line_tree->line_type != ERR_LINE)
		{
			ast_set_line_error("Illegal operand", line_tree);
		}
		return TRUE;
	}
	return FALSE;
}

int how_many_operands_need(char *str)
{
	if(is_zero_op_inst(str))
	{
		return 0;
	}
	
	if(is_one_op_inst(str))
	{
		return 1;
	}
	
	return 2;
}

boolean is_inst(char *str)
{
	return is_two_op_inst(str) || is_one_op_inst(str) || is_zero_op_inst(str);
}

void set_inst_type(char *inst, struct astNode *line_tree)
{
	if (!strcmp(inst, "mov"))
		line_tree->line_content.instruction.inst_opcode = MOV_INST;
	else if (!strcmp(inst, "cmp"))
		line_tree->line_content.instruction.inst_opcode = CMP_INST;
	else if (!strcmp(inst, "add"))
		line_tree->line_content.instruction.inst_opcode = ADD_INST;
	else if (!strcmp(inst, "sub"))
		line_tree->line_content.instruction.inst_opcode = SUB_INST;
	else if (!strcmp(inst, "lea"))
		line_tree->line_content.instruction.inst_opcode = LEA_INST;
	else if (!strcmp(inst, "not"))
		line_tree->line_content.instruction.inst_opcode = NOT_INST;
	else if (!strcmp(inst, "clr"))
		line_tree->line_content.instruction.inst_opcode = CLR_INST;
	else if (!strcmp(inst, "inc"))
		line_tree->line_content.instruction.inst_opcode = INC_INST;
	else if (!strcmp(inst, "dec"))
		line_tree->line_content.instruction.inst_opcode = DEC_INST;
	else if (!strcmp(inst, "jmp"))
		line_tree->line_content.instruction.inst_opcode = JMP_INST;
	else if (!strcmp(inst, "bne"))
		line_tree->line_content.instruction.inst_opcode = BNE_INST;
	else if (!strcmp(inst, "red"))
		line_tree->line_content.instruction.inst_opcode = RED_INST;
	else if (!strcmp(inst, "prn"))
		line_tree->line_content.instruction.inst_opcode = PRN_INST;
	else if (!strcmp(inst, "jsr"))
		line_tree->line_content.instruction.inst_opcode = JSR_INST;
	else if (!strcmp(inst, "rts"))
		line_tree->line_content.instruction.inst_opcode = RTS_INST;
	else
		line_tree->line_content.instruction.inst_opcode = STOP_INST;
}
