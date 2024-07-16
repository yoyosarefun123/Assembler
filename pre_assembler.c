#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "dictionary.h"
#include "parsering_lines.h"
#include "pre_assembler.h"
#include "saved_names.h"
#include "bool.h"



int span_macros(FILE *file, char *filename, dict *mcro_table){
	char *name; /*The name of the file + am*/
	FILE *spanned_file; /*The file.am*/
	int currIndex; /*Current index, used for picking words from the line*/
	dict_item *currMcroContent; /*current macro's content*/
	int is_mcro; /*boolean flag - are we in the middle of a macro?*/
	char *currLine; /*The line we currently analyzing*/
	char *leftovers; /*a string that checks whether there are leftovers after the command or there are'nt*/
	int current_line; /*what line we at (numberwise)*/
	int error_cnt; /*how many errors we got*/
	char *firstWord;
	char *mcroName;
	char ch;

	error_cnt = 0;
	leftovers = NULL;	
	currIndex = 0;
	is_mcro = 0;
	current_line = 1;
	currLine = malloc(sizeof(char)*MAX_LINE_LENGTH+1);
	firstWord = malloc(sizeof(char)*MAX_LINE_LENGTH);
	mcroName = malloc(sizeof(char)*MAX_LINE_LENGTH);
	name = add_file_type(filename, ".am");		

	spanned_file = fopen(name, "w+");
	if(!spanned_file)
	{
		printf("Error! Failed to open the file!\n");
		exit(1);
	}


	while(fgets(currLine, MAX_LINE_LENGTH+1, file) != NULL)
	{
		
		getWord(currLine, &firstWord, &currIndex);
		if(!(is_mcro && !strcmp(firstWord, "endmcro")))
			if(strlen(currLine)>80)
			{
				printf("line %d is longer than 80 characters\n", current_line);
				error_cnt++;
				while((ch = fgetc(file))!='\n');
				currIndex=0;
				current_line++;
				continue;
			}
		

		
		if(!strcmp(firstWord, "") || !strncmp(firstWord, ";", 1))
		{;}
		else if(!strcmp(firstWord, "mcro"))
		{
			getWord(currLine, &mcroName, &currIndex);
			if(is_mcro_name_legal(mcroName, mcro_table))
			{
				is_mcro = 1;
				currMcroContent = (create_new_dict_item(mcroName));
			}
			else
			{
				printf("illegal macro name in line %d\n", current_line);
				error_cnt++;
			}
			if(strlen(currLine)>80)
			{
				printf("line %d is longer than 80 characters\n", current_line);
				error_cnt++;
				while((ch = fgetc(file))!='\n');
				currIndex=0;
				current_line++;
				continue;
			}
			getWord(currLine, &leftovers, &currIndex);

			if(leftovers)
			{
				printf("extraneous text after end of command in line %d, you wrote \"%s\"\n", current_line, leftovers);
				error_cnt++;
				free(leftovers);
				leftovers = NULL;
			}
		}
		else if(is_mcro)
		{

			if(!strcmp(firstWord, "endmcro"))
			{
				is_mcro = 0;

				put_dict(mcro_table, currMcroContent);
				
				if(strlen(currLine)>80)
				{
					printf("line %d is longer than 80 characters\n", current_line);
					error_cnt++;
					while((ch = fgetc(file))!='\n');
					currIndex=0;
					current_line++;
					continue;
				}
				
				getWord(currLine, &leftovers, &currIndex);
				if(leftovers)
				{
					printf("extraneous text after end of command in line %d, you wrote \"%s\"\n", current_line, leftovers);
					error_cnt++;
					free(leftovers);
					leftovers = NULL;
				}
			}			
			else add2mcro(currLine, currMcroContent);
		}
		
		else if(is_mcro_call(mcro_table, firstWord))
		{
			print_mcro(spanned_file, mcro_table, firstWord);

		}
		else 
		{
			fputs(currLine, spanned_file);

		}
		currIndex=0;
		current_line++;
		/*free(currLine);
		currLine = (char*)malloc(sizeof(char)*82);*/
	}
	if(firstWord!=NULL) free(firstWord);
	if (currLine!=NULL) free(currLine);
	if(mcroName!=NULL) free(mcroName);
	fclose(spanned_file);
	if(error_cnt)
	{
		remove(name);
		printf("could'nt create a .am file because of errors in macro definitions\n");
	}
	free(name);
	return error_cnt;
}


void print_mcro(FILE *file, dict *mcro_table, char *name) {
	linked_list *macro;
	macro = get_dict_value(mcro_table, name);
	print_list_to_file(macro, file);
}

void add2mcro(char *currLine, dict_item *mcroContent) {
	node *new_line = create_new_node();
	node_set_value(new_line, currLine);
	
	add_node_to_dict_item_list(mcroContent, new_line);
}


boolean is_mcro_name_legal(char *name, dict *mcro_table) {
	if(is_mcro_name(name, mcro_table) || is_saved_name(name))
		return FALSE;
	return TRUE;
}

boolean is_mcro_name(char *name, dict *mcro_table)
{
	return key_exists(mcro_table, name);
}

boolean is_mcro_call(dict *mcro_table, char *name) {
	if(key_exists(mcro_table, name))
		return TRUE;
	return FALSE;
}
