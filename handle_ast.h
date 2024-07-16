#ifndef HANDLE_AST_H
#define HANDLE_AST_H

#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "bool.h"
#include "linked_list.h"
#include "dictionary.h"

/** Structure representing a machine code line.
 *  A machine code line contains either a label or data.
 */
typedef struct machine_code_line {
    int data_type; /**< Type of data in the line (0 for label, 1 for data). */
    union {
        char label[31]; /**< Label content (up to 30 characters). */
        int data; /**< Data content. */
    } content;
} machine_code_line;

/** Handle an abstract syntax tree (AST) node representing a line of assembly code.
 * 
 * This function processes an AST node representing a line of assembly code and generates corresponding machine code.
 *
 * @param curr_line The current AST node to be handled.
 * @param label_table A pointer to the linked list of label entries.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param data_img A pointer to the linked list representing the machine code image for data.
 * @param DC A pointer to the data counter, which is updated during processing.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 * @param line_cnt The current line number in the source code.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 * @param mcro_table The macro dictionary containing macro definitions.
 */
void handle_ast(astNode* curr_line, linked_list** label_table, machine_code_line** code_img,
                machine_code_line** data_img, int* DC, int* IC, int line_cnt, int* error_cnt, dict* mcro_table);

/** Handle an assembly directive AST node.
 * 
 * This function processes an AST node representing an assembly directive and performs the required actions.
 *
 * @param curr_line The current AST node representing the directive to be handled.
 * @param label_table A pointer to the linked list of label entries.
 * @param data_img A pointer to the linked list representing the machine code image for data.
 * @param DC A pointer to the data counter, which is updated during processing.
 * @param line_cnt The current line number in the source code.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 */
void handle_directive(astNode* curr_line, linked_list** label_table, machine_code_line** data_img,
                      int* DC, int line_cnt, int* error_cnt);

/** Handle a data directive AST node.
 * 
 * This function processes an AST node representing a data directive and updates the data image accordingly.
 *
 * @param curr_line The current AST node representing the data directive to be handled.
 * @param label_table A pointer to the linked list of label entries.
 * @param data_img A pointer to the linked list representing the machine code image for data.
 * @param DC A pointer to the data counter, which is updated during processing.
 * @param line_cnt The current line number in the source code.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 */
void handle_data_dir(astNode* curr_line, linked_list** label_table, machine_code_line** data_img,
                     int* DC, int line_cnt, int* error_cnt);

/** Update the entry flag for a label in the label table.
 * 
 * This function updates the entry flag for a label in the label table, indicating it is an entry label.
 *
 * @param label_table A pointer to the linked list of label entries.
 * @param label The label for which the entry flag should be updated.
 */
void update_entry(linked_list** label_table, char* label);

/** Check if a label is in the label table.
 * 
 * This function checks if a label exists in the label table.
 *
 * @param label_table A pointer to the linked list of label entries.
 * @param label The label to search for.
 * @return True if the label is found in the table, False otherwise.
 */
boolean in_label_table(linked_list** label_table, char* label);

/** Check if an entry label is declared.
 * 
 * This function checks if an entry label is declared.
 *
 * @param ent_list A pointer to the linked list of entry label entries.
 * @param label The entry label to check.
 * @return True if the entry label is declared, False otherwise.
 */
boolean declared_entry(linked_list* ent_list, char* label);

/** Check if an extern label is declared.
 * 
 * This function checks if an extern label is declared.
 *
 * @param ext_list A pointer to the linked list of extern label entries.
 * @param label The extern label to check.
 * @return True if the extern label is declared, False otherwise.
 */
boolean declared_extern(linked_list* ext_list, char* label);

/** Check if a label corresponds to a macro.
 * 
 * This function checks if a label corresponds to a macro in the macro dictionary.
 *
 * @param label The label to check.
 * @param mcro_table The macro dictionary.
 * @return True if the label is a macro, False otherwise.
 */
boolean is_label_mcro(char* label, dict* mcro_table);

/** Handle a string directive AST node.
 * 
 * This function processes an AST node representing a string directive and updates the data image accordingly.
 *
 * @param curr_line The current AST node representing the string directive to be handled.
 * @param label_table A pointer to the linked list of label entries.
 * @param data_img A pointer to the linked list representing the machine code image for data.
 * @param DC A pointer to the data counter, which is updated during processing.
 * @param line_cnt The current line number in the source code.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 */
void handle_string_dir(astNode* curr_line, linked_list** label_table, machine_code_line** data_img,
                       int* DC, int line_cnt, int* error_cnt);

/** Handle an entry directive AST node.
 * 
 * This function processes an AST node representing an entry directive and updates the label entry accordingly.
 *
 * @param curr_line The current AST node representing the entry directive to be handled.
 * @param label_table A pointer to the linked list of label entries.
 * @param DC A pointer to the data counter, which is updated during processing.
 * @param line_cnt The current line number in the source code.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 */
void handle_ent_dir(astNode* curr_line, linked_list** label_table, int* DC, int line_cnt, int* error_cnt);

/** Handle an extern directive AST node.
 * 
 * This function processes an AST node representing an extern directive and updates the label entry accordingly.
 *
 * @param curr_line The current AST node representing the extern directive to be handled.
 * @param label_table A pointer to the linked list of label entries.
 * @param DC A pointer to the data counter, which is updated during processing.
 * @param line_cnt The current line number in the source code.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 */
void handle_ext_dir(astNode* curr_line, linked_list** label_table, int* DC, int line_cnt, int* error_cnt);

/** Handle an instruction AST node.
 * 
 * This function processes an AST node representing an instruction and updates the code image accordingly.
 *
 * @param curr_line The current AST node representing the instruction to be handled.
 * @param label_table A pointer to the linked list of label entries.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 * @param line_cnt The current line number in the source code.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 */
void handle_instruction(astNode* curr_line, linked_list** label_table, machine_code_line** code_img,
                        int* IC, int line_cnt, int* error_cnt);

/** Handle a two-operand instruction AST node.
 * 
 * This function processes an AST node representing a two-operand instruction and updates the code image accordingly.
 *
 * @param curr_line The current AST node representing the two-operand instruction to be handled.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 */
void handle_two_op_instruction(astNode* curr_line, machine_code_line** code_img, int* IC);

/** Handle a one-operand instruction AST node.
 * 
 * This function processes an AST node representing a one-operand instruction and updates the code image accordingly.
 *
 * @param curr_line The current AST node representing the one-operand instruction to be handled.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 */
void handle_one_op_instruction(astNode* curr_line, machine_code_line** code_img, int* IC);

/** Handle a zero-operand instruction AST node.
 * 
 * This function processes an AST node representing a zero-operand instruction and updates the code image accordingly.
 *
 * @param curr_line The current AST node representing the zero-operand instruction to be handled.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 */
void handle_zero_op_instruction(astNode* curr_line, machine_code_line** code_img, int* IC);

/** Add source operand value for a two-operand instruction.
 * 
 * This function adds the source operand value for a two-operand instruction to the code image.
 *
 * @param curr_line The current AST node representing the instruction.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 */
void add_twoop_src_value(astNode* curr_line, machine_code_line** code_img, int* IC);

/** Add target operand value for a two-operand instruction.
 * 
 * This function adds the target operand value for a two-operand instruction to the code image.
 *
 * @param curr_line The current AST node representing the instruction.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 */
void add_twoop_trg_value(astNode* curr_line, machine_code_line** code_img, int* IC);

/** Add target operand value for a one-operand instruction.
 * 
 * This function adds the target operand value for a one-operand instruction to the code image.
 *
 * @param curr_line The current AST node representing the instruction.
 * @param code_img A pointer to the linked list representing the machine code image for instructions.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 */
void add_oneop_trg_value(astNode* curr_line, machine_code_line** code_img, int* IC);

/** Check for errors in an instruction.
 * 
 * This function checks if an instruction matches the expected opcode and operands, reporting any errors.
 *
 * @param curr_line The current AST node representing the instruction.
 * @param opcode The expected opcode for the instruction.
 * @param line_cnt The current line number in the source code.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 * @return True if an error is detected, False otherwise.
 */
boolean error_instruction(astNode* curr_line, int opcode, int line_cnt, int* error_cnt);

#endif 