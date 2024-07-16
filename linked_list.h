#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include "bool.h"

/** Structure representing a node in a linked list.
 *  A node contains data and a reference to the next node in the list.
 */
typedef struct node node;

/** Structure representing a linked list.
 *  A linked list is a collection of nodes.
 */
typedef struct linked_list linked_list;

/** Update the position of a node in the linked list based on the value.
 * 
 * This function updates the position of a node in the linked list based on the value it holds.
 *
 * @param list The linked list in which to search for the node.
 * @param val The value to be matched against node values.
 * @param pos The new position to set for the matched node.
 */
void update_position_by_value(linked_list* list, char* val, int pos);

/** Check if there are undeclared entry labels in a linked list.
 * 
 * This function checks if there are undeclared entry labels in a given linked list.
 *
 * @param ent The linked list of entry label entries to check.
 * @param error_cnt A pointer to the error count, which is updated with any encountered errors.
 * @return True if undeclared entry labels are found, False otherwise.
 */
boolean check_undeclared_ent(linked_list* ent, int* error_cnt);

/** Get the position of a node in the linked list based on the value.
 * 
 * This function retrieves the position of a node in the linked list based on the value it holds.
 *
 * @param list The linked list in which to search for the node.
 * @param val The value to be matched against node values.
 * @return The position of the matched node, or -1 if not found.
 */
int get_position_by_value(linked_list* list, char* val);

/** Create a new node.
 * 
 * This function creates a new node with default values.
 *
 * @return A pointer to the newly created node.
 */
node* create_new_node();

/** Create a new node with a specified value.
 * 
 * This function creates a new node with the specified value.
 *
 * @param val The value to be assigned to the new node.
 * @return A pointer to the newly created node.
 */
node* create_node(char* val);

/** Set the position of a node.
 * 
 * This function sets the position of a node in the linked list.
 *
 * @param n The node for which to set the position.
 * @param pos The position to assign to the node.
 */
void node_set_position(struct node* n, int pos);

/** Set the value of a node.
 * 
 * This function sets the value of a node in the linked list.
 *
 * @param n The node for which to set the value.
 * @param new_val The new value to assign to the node.
 */
void node_set_value(node* n, char* new_val);

/** Create a new linked list.
 * 
 * This function creates a new, empty linked list.
 *
 * @return A pointer to the newly created linked list.
 */
linked_list* create_new_list();

/** Create a copy of a linked list.
 * 
 * This function creates a copy of a given linked list.
 *
 * @param src The linked list to be copied.
 * @return A pointer to the newly created copy of the linked list.
 */
linked_list* copy_list(linked_list* src);

/** Add a node to a linked list.
 * 
 * This function adds a node to the end of a linked list.
 *
 * @param list The linked list to which the node is to be added.
 * @param item The node to be added to the linked list.
 */
void add_node(linked_list* list, node* item);

/** Update the data position for nodes in a linked list.
 * 
 * This function updates the data position for nodes in a linked list.
 *
 * @param label_table A pointer to the linked list of label entries.
 * @param DC A pointer to the data counter, which is updated during processing.
 * @param IC A pointer to the instruction counter, which is updated during processing.
 */
void update_data_position(linked_list** label_table, int* IC);

/** Get the position of a node.
 * 
 * This function retrieves the position of a node in the linked list.
 *
 * @param n The node for which to get the position.
 * @return The position of the node.
 */
int list_get_position(node* n);

/** Get the length of a linked list.
 * 
 * This function returns the number of nodes in a linked list.
 *
 * @param list The linked list for which to get the length.
 * @return The length of the linked list.
 */
int list_length(linked_list* list);

/** Print the contents of a linked list to a file.
 * 
 * This function prints the contents of a linked list to a specified file.
 *
 * @param list The linked list to be printed.
 * @param file The file to which the linked list contents should be printed.
 */
void print_list_to_file(linked_list* list, FILE* file);

/** Free the memory occupied by a linked list.
 * 
 * This function frees the memory occupied by a linked list and its nodes.
 *
 * @param list The linked list to be freed.
 */
void free_list(linked_list* list);

/** Check if a value exists in a linked list.
 * 
 * This function checks if a value exists in a linked list.
 *
 * @param list The linked list in which to search for the value.
 * @param word The value to be searched for.
 * @return True if the value exists in the linked list, False otherwise.
 */
int value_exists(linked_list* list, char* word);

/** Search for a label in a linked list.
 * 
 * This function searches for a label in a linked list.
 *
 * @param label The label to search for.
 * @param list The linked list to search within.
 * @return The position of the label in the linked list, or -1 if not found.
 */
int search_label(char* label, linked_list* list);

#endif