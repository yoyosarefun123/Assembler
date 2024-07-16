#include "handle_ast.h"
#include "linked_list.h"

/**
 * Perform the second pass of the assembly process.
 * This function generates the output files in the object file format.
 *
 * @param file_name     The name of the source file.
 * @param code_img      Pointer to an array of machine_code_line representing the code image.
 * @param data_img      Pointer to an array of machine_code_line representing the data image.
 * @param label_table   Pointer to an array of linked_list pointers representing the label table.
 * @param ext_img       Pointer to the linked_list storing the external symbols.
 * @param DC            Data Counter (data segment size).
 * @param IC            Instruction Counter (code segment size).
 * @param error_cnt     Pointer to an integer to keep track of the error count.
 */
 
void second_pass(char* file_name, machine_code_line** code_img, machine_code_line** data_img, linked_list** label_table, linked_list* ext_img, int* DC, int* IC, int* error_cnt);
    
/**
 * Convert a label to machine code using the label table.
 * This function converts a label to machine code based on its position in the label table.
 *
 * @param label         The label to be converted.
 * @param label_table   Pointer to an array of linked_list pointers representing the label table.
 * @param ext_img       Pointer to the linked_list storing the external symbols.
 * @param pos           The position of the label in the code image.
 * @return              The machine code representation of the label, or -1 if the label is not found.
 */

int label_to_code(char* label, linked_list** label_table, linked_list* ext_img, int pos);

