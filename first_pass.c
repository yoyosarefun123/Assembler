#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "handle_ast.h"
#include "linked_list.h"
#include "ast.h"
#include "dictionary.h"

#define LINE_MAX_SIZE 81
/*the central function of the first pass of our assembler. for each line in the file, it builds an AST, 
reads the necessary information from it and adds to the different data structures to be printed out to 
the file or used later on in the second pass.*/

int first_pass(FILE* file, linked_list** label_table, machine_code_line** code_img, machine_code_line** data_img, int* DC, int* IC, dict* mcro_table, int* error_cnt) {
	int line_cnt = 1;
	char *line = (char*)malloc(sizeof(char) * LINE_MAX_SIZE);
    astNode *line_tree;
	/*read each line from the file (maximum size of line is 80 characters)
	build the ast and create the data accordingly*/
	while(fgets(line, LINE_MAX_SIZE, file) != NULL) {
		line_tree = build_ast_from_line(line);
		handle_ast(line_tree, label_table, code_img, data_img, DC, IC, line_cnt, error_cnt, mcro_table);
		free(line_tree);
		line_cnt++;
	}
	update_data_position(label_table, IC);
	free(line);
	if((*error_cnt)) {
        printf("There are %d errors in your program after first run\n", *error_cnt);
    }
	return *error_cnt;
}
