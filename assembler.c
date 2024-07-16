#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "bool.h"
#include "dictionary.h"
#include "linked_list.h"
#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "parsering_lines.h"
#include "handle_ast.h"

/*the main function of the program - takes the input file and creates output files (or prints errors)*/
void assembler(char *file_name)
{
    /*variable definition*/
	FILE *file; 
	char *filename;
	int first_pass_result;	
	char *line;
    int IC = 0;
	int DC = 0;
    int error_cnt = 0;
	dict *mcro_table; /*a table that contains all the macros*/
	linked_list* data;
	linked_list* ent;
	linked_list* ext;
	linked_list* code;
	linked_list** label_table;
    machine_code_line *code_img;
	machine_code_line *data_img;
    linked_list* ext_img;
	filename = NULL;
	
	line = (char*)malloc(sizeof(char) * 81);
	filename = add_file_type(file_name, ".as");		
	file = fopen(filename, "r+"); 
	if(!file)
	{
		printf("Error! Failed to open file \"%s\"!\n", filename);
		free(line);
		free(filename);
		exit(1);
	}
    /*use the pre-assembler - create a new dict that will save the macro names, and span them 
    (span_macros is the main function of the pre-assembler, and returns the .am file requested)*/
	mcro_table = create_new_dict();
	
    /*spans the macros, if there are errors end the run (span_macros returns the error count)*/
	if (span_macros(file, file_name, mcro_table)) {
        fclose(file);
	    free(filename);
        return;
    }
	fclose(file);
	free(filename);
    
    /*start the first pass of the assembler (if the pre-assembler finished its job properly)*/
	
    /*creating the label table טבלת סמלים in four seperate linked lists as we found it easier to 
    split it according to if it's a data/string label, an entry, extern or code*/
    data = create_new_list();
	ent = create_new_list();
	ext = create_new_list();
	code = create_new_list();
	label_table = (linked_list**)malloc (4 * sizeof(linked_list*));
	label_table[0] = data;
	label_table[1] = ent;
	label_table[2] = ext;
	label_table[3] = code;
	/*creating the struct that holds the information for the final code (explained in handle_ast where it's defined)*/
    code_img = malloc(924 * sizeof(machine_code_line));
	data_img = malloc(924 * sizeof(machine_code_line));
    
    filename = add_file_type(file_name, ".am");
	file = fopen(filename, "r+");
	first_pass_result = (first_pass(file, label_table, &code_img, &data_img, &DC, &IC, mcro_table, &error_cnt));
    /*first_pass_result is nonzero if there was an error in the first pass, as the return value of 
    first_pass is the amount of errors. If it is nonzero, we free all the information we used and exit the program.*/
	if(first_pass_result)
	{
		
		free(line);
		fclose(file);
		free_dict(mcro_table);
		free(filename);
		free_list(data);
		free(data);
		free_list(ent);
		free(ent);
		free_list(ext);
		free(ext);
		free_list(code);
		free(code);
		free(label_table);
		free(code_img);
		free(data_img);
		return;
	}

    /*start doing the second pass - creating a new list called ext_img that will hold the values in the 
    memory where there were referalls to labels defined as external labels*/
	ext_img = create_new_list();
    /*second pass does not have a return value. instead if there are errors, 
    we just delete the output files and exit the program. after it completes running,
    we free all the memory relevant regardless of if there was an error or not.*/
	second_pass(file_name, &code_img, &data_img, label_table, ext_img, &DC, &IC, &error_cnt);
	free(line);
	fclose(file);
	free_dict(mcro_table);
	free(filename);
	free_list(data);
	free(data);
	free_list(ent);
	free(ent);
	free_list(ext);
	free(ext);
	free_list(code);
	free(code);
	free(label_table);
	free(code_img);
	free(data_img);
	free_list(ext_img);
	free(ext_img);
}