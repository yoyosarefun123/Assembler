#ifndef SAVED_NAMES_H
#define SAVED_NAMES_H

#include "bool.h"
#include "saved_names.h"

/** Check if a given word is a saved name.
 * 
 * This function checks if a given word is a saved name, which includes instructions, directives, and other reserved words.
 *
 * @param word The word to check.
 * @return True if the word is a saved name, False otherwise.
 */
boolean is_saved_name(char *word);

/** Check if a given word corresponds to a macro or an instruction.
 * 
 * This function checks if a given word corresponds to either a macro or an instruction.
 *
 * @param word The word to check.
 * @return True if the word corresponds to a macro or an instruction, False otherwise.
 */
boolean is_mcro_inst(char *word);

/** Check if a given word corresponds to an assembly directive.
 * 
 * This function checks if a given word corresponds to an assembly directive.
 *
 * @param word The word to check.
 * @return True if the word corresponds to an assembly directive, False otherwise.
 */
boolean is_directive(char *word);

/** Check if a given word corresponds to a two-operand instruction.
 * 
 * This function checks if a given word corresponds to a two-operand instruction.
 *
 * @param word The word to check.
 * @return True if the word corresponds to a two-operand instruction, False otherwise.
 */
boolean is_two_op_inst(char *word);

/** Check if a given word corresponds to a one-operand instruction.
 * 
 * This function checks if a given word corresponds to a one-operand instruction.
 *
 * @param word The word to check.
 * @return True if the word corresponds to a one-operand instruction, False otherwise.
 */
boolean is_one_op_inst(char *word);

/** Check if a given word corresponds to a zero-operand instruction.
 * 
 * This function checks if a given word corresponds to a zero-operand instruction.
 *
 * @param word The word to check.
 * @return True if the word corresponds to a zero-operand instruction, False otherwise.
 */
boolean is_zero_op_inst(char *word);

#endif