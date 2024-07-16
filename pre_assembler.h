#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "bool.h"
#include "linked_list.h"
#include "dictionary.h"
#include "stdio.h"

/** Maximum line length for reading from the input file. */
#define MAX_LINE_LENGTH 81

/** Span macros in an input file and expand them.
 * 
 * This function scans an input file for macro invocations, expands them, and produces an output file with expanded content.
 *
 * @param file The input file to be processed.
 * @param filename The name of the input file.
 * @param mcro_table The dictionary containing macro definitions.
 * @return The number of errors encountered during preprocessing.
 */
int span_macros(FILE *file, char *filename, dict *mcro_table);

/** Print the content of a macro definition to a file.
 * 
 * This function prints the content of a macro definition to the specified output file.
 *
 * @param file The output file to which the macro content should be printed.
 * @param mcro_table The dictionary containing macro definitions.
 * @param name The name of the macro to be printed.
 */
void print_mcro(FILE *file, dict *mcro_table, char *name);

/** Add a line to a macro's content.
 * 
 * This function adds a line to the content of a macro definition.
 *
 * @param currLine The line to be added to the macro content.
 * @param mcroContent The dictionary item representing the macro's content.
 */
void add2mcro(char *currLine, dict_item *mcroContent);

/** Check if a name corresponds to a macro.
 * 
 * This function checks if a given name corresponds to a defined macro in the macro dictionary.
 *
 * @param name The name to check.
 * @param mcro_table The dictionary containing macro definitions.
 * @return True if the name corresponds to a macro, False otherwise.
 */
boolean is_mcro_name(char *name, dict *mcro_table);

/** Check if a macro name is legal and not a keyword.
 * 
 * This function checks if a macro name is legal and not a keyword.
 *
 * @param name The name of the macro to check.
 * @param mcro_table The dictionary containing macro definitions.
 * @return True if the macro name is legal, False otherwise.
 */
boolean is_mcro_name_legal(char *name, dict *mcro_table);

/** Check if a given name corresponds to a macro call.
 * 
 * This function checks if a given name corresponds to a macro call by looking it up in the macro dictionary.
 *
 * @param mcro_table The dictionary containing macro definitions.
 * @param name The name to check.
 * @return True if the name corresponds to a macro call, False otherwise.
 */
boolean is_mcro_call(dict *mcro_table, char *name);

#endif