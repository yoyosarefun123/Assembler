#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parsering_lines.h"
#include "bool.h"
#include "ast.h"



void getWord(char *input, char **currStr, int *i)
{
	int indx;
	int didReceived; /*this will indicate if we even got a command*/	
	
	didReceived=0;
	indx=0;

	skipWhites(input, i); /*skips the first white space*/

	while(input[*i]!='\0' && input[*i]!='\t' && input[*i]!='\n' && input[*i]!=' ' && input[*i]!=',')/*while we are still in the bounds of the string & not get white space*/
	{
		(*currStr)[indx]=input[*i];
		(*i)++; /*we'd put the character in the currStr and move to next index*/
		indx++;
		didReceived=1; /*if we entered the loop, we received something*/
	}
	
	if(didReceived) (*currStr)[indx] = '\0';
	else *i = -1;


}

boolean is_there_comma(char *line, int *index)
{
	int i =  *index;
	
	skipWhites(line, &i);
	if(line[i]==',')
	{
	*index = i+1; /*increase by one, this one is the comma*/
	return TRUE;
	}
	*index = i;
	return FALSE;
}

char next_char(char *str, int i)
{
	skipWhites(str, &i);
	return str[i];
}

int getNum(char *line, int *curr_data, int *index, int is_purpose_data, astNode *line_tree)
{
	int i, is_neg, j;
	char num[4]; /*the numbers i can get are from -2048 to 2047 (or from -512 to 511) so i dont need a bigger array*/
	for(j=0;j<=3;j++)
		num[j]='\0';
	i = *index;
	is_neg = 0;

	skipWhites(line, &i);
	
	if(line[i]=='-')
	{
		is_neg = 1;
		i++;
	}
	else if(line[i]=='+')
	{
		i++; /*a + does not affect the number itself*/
	}
	
	if(line[i]=='\n' || line[i]=='\0')
	{
		ast_set_line_error("Missing parameter", line_tree);
		return 1;
	}
	if(line[i]==',')
	{
		ast_set_line_error("Illegal comma", line_tree);
		return 1;
	}
	
	if(is_purpose_data)
	{
		for (j=0; j<=3; j++)
		{
			if(isdigit(line[i]))
			{
				num[j]=line[i];
				i++;
			}
			else
			{
				if(line[i]!=',' && line[i]!=' ' && line[i]!='\t' && line[i]!='\0' && line[i]!='\n')
				{
					ast_set_line_error("Illegal operand for directive '.data'", line_tree);
					return 1;
				}
				else break;
			}
		}
	}
	else
	{
		for (j=0; j<=2; j++)
		{
			if(isdigit(line[i]))
			{
				num[j]=line[i];
				i++;
			}
			else
			{
				if(line[i]!=',' && line[i]!=' ' && line[i]!='\t' && line[i]!='\0' && line[i]!='\n')
				{
					ast_set_line_error("Illegal operand for instruction", line_tree);
					return 1;
				}
				else break;
			}
		}
	}
	if(isdigit(line[i])) /*in that case there is a 5 digits long number (or 4 digit) and it's too big for our purpose*/
	{
		ast_set_line_error("Integer overflow", line_tree);
		return 1;
	}
	if(line[i]!=',' && line[i]!=' ' && line[i]!='\t' && line[i]!='\0' && line[i]!='\n') /*its something line 23fmivme*/
	{
		ast_set_line_error("Illegal operand", line_tree);
		return 1;
	}
	*index = i;
	*curr_data = atoi(num);
	if(is_neg) *curr_data *= -1;
	
	if(!is_purpose_data)
	{
		if((*curr_data)>MAX_INT_10BITS || (*curr_data)<MIN_INT_10BITS)
		{
			ast_set_line_error("Integer overflow", line_tree);
			return 1;
		}
	}
	else
	{
		if((*curr_data)>MAX_INT_12BITS || (*curr_data)<MIN_INT_12BITS)
		{
			ast_set_line_error("Integer overflow", line_tree);
			return 1;
		}
	}
	
	return 0;
	
	
}

void skipWhites(char *input, int *currIndex)
{	
	if(!input)
	{
		return;
	}

	while(input[*currIndex]!='\0' && (input[*currIndex]=='\t' || input[*currIndex]==' ' || input[*currIndex]=='\n'))/*we will run until we'll get a non-white character*/
	{
		(*currIndex)++;
	}

}


char* add_file_type(char *name, char *file_type) {
	int len;
	int len_type;
	char *name_and_type;
	
	len = strlen(name);
	len_type = len + strlen(file_type) + 1; /*add the length of the file_type to the name, plus 1 for '\0'*/
	name_and_type = (char*)malloc(sizeof(char)*len_type);
	strcpy(name_and_type, name);
	strcat(name_and_type, file_type);	
	return name_and_type;
}