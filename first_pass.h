#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "handle_ast.h"

/** Perform the first pass of the assembly process.
 * 
 * This function performs the first pass of the assembly process on the given input file.
 * It processes the source code, collects information about labels, and generates
 * intermediate machine code instructions for the assembly process.
 *
 * @param file The input file containing the assembly source code.
 * @param label_table A pointer to a linked list of label entries to be populated during the pass.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param data_img A pointer to the linked list representing the machine code image for data.
 * @param DC A pointer to the data counter, which is updated during processing.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 * @param mcro_table The macro dictionary containing macro definitions.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 * @return The exit code for the first pass (0 for success, non-zero for errors).
 */
int first_pass(FILE* file, linked_list** label_table, machine_code_line** code_img,
               machine_code_line** data_img, int* DC, int* IC, dict* mcro_table, int* error_cnt);

#endif