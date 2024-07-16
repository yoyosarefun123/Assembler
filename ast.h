#ifndef AST_H
#define AST_H
#include "bool.h"
/**
this struct represents an AST for this assembly language.
the AST is a data structure that contains the data from a line of the code
it makes the coding of these lines much easier and has much more benefits.
we chose to work with it in the project because it makes our life much easier.
*/
typedef struct astNode astNode;
/**
 * Get the type of the line in an AST node.
 *
 * @param node Pointer to the AST node to examine.
 * @return The type of the line, which can be ERR_LINE, INST_LINE, DIR_LINE, or EMPTY_LINE.
 */
int ast_get_line_type(astNode *node);

/**
 * Check if the given AST node is labeled.
 *
 * @param tree Pointer to the AST node to check.
 * @return TRUE if the node is labeled, FALSE otherwise.
 */
boolean ast_is_label(astNode *tree);

/**
 * Get the error message associated with the given AST node's line.
 *
 * @param node Pointer to the AST node to examine.
 * @return Pointer to the error message string, or NULL if no error is present.
 */
char* ast_get_line_error(astNode *node);

/**
 * Set an error message for the given AST node's line.
 *
 * @param str Error message string to set.
 * @param tree Pointer to the AST node to which the error message is associated.
 */
void ast_set_line_error(char *str, astNode *tree);

/**
 * Get the label associated with the given AST node's line.
 *
 * @param node Pointer to the AST node to examine.
 * @return Pointer to the label string, or NULL if no label is present.
 */
char* ast_get_line_label(astNode *node);

/**
 * Get the type of directive in the given AST node.
 *
 * @param node Pointer to the AST node representing a directive.
 * @return The directive type, which can be DATA, STR, ENT, or EXT.
 */
int ast_dir_get_type(astNode *node);

/**
 * Get a pointer to the array of numbers in a DATA directive AST node.
 *
 * @param node Pointer to the AST node representing a DATA directive.
 * @return Pointer to the array of numbers, or NULL if not applicable.
 */
int* ast_dir_data_get_num_array(astNode *node);

/**
 * Get the count of numbers in a DATA directive AST node.
 *
 * @param node Pointer to the AST node representing a DATA directive.
 * @return The count of numbers in the DATA directive.
 */
int ast_dir_data_get_num_cnt(astNode *node);

/**
 * Get the string content in a STR directive AST node.
 *
 * @param node Pointer to the AST node representing a STR directive.
 * @return Pointer to the string content, or NULL if not applicable.
 */
char* ast_dir_get_string(astNode *node);

/**
 * Get a pointer to the array of labels in an EXT or ENT directive AST node.
 *
 * @param node Pointer to the AST node representing an EXT or ENT directive.
 * @return Pointer to the array of labels, or NULL if not applicable.
 */
char** ast_dir_get_labels(astNode *node);

/**
 * Get the count of labels in an EXT or ENT directive AST node.
 *
 * @param node Pointer to the AST node representing an EXT or ENT directive.
 * @return The count of labels in the EXT or ENT directive.
 */
int ast_dir_get_label_cnt(astNode *node);

/**
 * Get the opcode of the instruction in the given AST node.
 *
 * @param node Pointer to the AST node representing an instruction.
 * @return The instruction opcode.
 */
int ast_inst_get_opcode(astNode *node);

/**
 * Get the address type of the operand in a one-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a one-operand instruction.
 * @return The address type of the operand.
 */
int ast_inst_oneop_get_addr(astNode *node);

/**
 * Get the immediate value of the operand in a one-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a one-operand instruction.
 * @return The immediate value of the operand.
 */
int ast_inst_oneop_get_immed(astNode *node);

/**
 * Get the label of the operand in a one-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a one-operand instruction.
 * @return Pointer to the label string, or NULL if not applicable.
 */
char* ast_inst_oneop_get_label(astNode *node);

/**
 * Get the register value of the operand in a one-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a one-operand instruction.
 * @return The register value of the operand.
 */
int ast_inst_oneop_get_reg(astNode *node);

/**
 * Get the address type of the source operand in a two-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a two-operand instruction.
 * @return The address type of the source operand.
 */
int ast_inst_twoop_src_get_addr(astNode *node);

/**
 * Get the address type of the target operand in a two-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a two-operand instruction.
 * @return The address type of the target operand.
 */
int ast_inst_twoop_trg_get_addr(astNode *node);

/**
 * Get the immediate value of the source operand in a two-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a two-operand instruction.
 * @return The immediate value of the source operand.
 */
int ast_inst_twoop_src_get_immed(astNode *node);

/**
 * Get the immediate value of the target operand in a two-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a two-operand instruction.
 * @return The immediate value of the target operand.
 */
int ast_inst_twoop_trg_get_immed(astNode *node);

/**
 * Get the label of the source operand in a two-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a two-operand instruction.
 * @return Pointer to the label string of the source operand, or NULL if not applicable.
 */
char* ast_inst_twoop_src_get_label(astNode *node);

/**
 * Get the label of the target operand in a two-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a two-operand instruction.
 * @return Pointer to the label string of the target operand, or NULL if not applicable.
 */
char* ast_inst_twoop_trg_get_label(astNode *node);

/**
 * Get the register value of the source operand in a two-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a two-operand instruction.
 * @return The register value of the source operand.
 */
int ast_inst_twoop_src_get_reg(astNode *node);

/**
 * Get the register value of the target operand in a two-operand instruction AST node.
 *
 * @param node Pointer to the AST node representing a two-operand instruction.
 * @return The register value of the target operand.
 */
int ast_inst_twoop_trg_get_reg(astNode *node);


boolean is_mcro_name_n(char *str);

/**
this function gets a line and builds an ast from it
the ast contains the data from the line in an organized way
*/
astNode* build_ast_from_line(char* line);
#endif