#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>
#include "linked_list.h"
#include "bool.h"

/** Structure representing a dictionary item.
 *  A dictionary item holds a key and a linked list of values associated with that key.
 */
typedef struct dict_item dict_item;

/** Structure representing a dictionary.
 *  A dictionary is a collection of dictionary items.
 */
typedef struct dict dict;

/** Add a node to the linked list of values in a dictionary item.
 * 
 * This function adds a node to the linked list of values associated with a dictionary item.
 *
 * @param di The dictionary item to which the node is to be added.
 * @param n The node to be added to the linked list.
 */
void add_node_to_dict_item_list(struct dict_item *di, node *n);

/** Check if a key exists in the dictionary.
 * 
 * This function checks whether a given key exists in the dictionary.
 *
 * @param dict The dictionary in which to search for the key.
 * @param key The key to search for.
 * @return True if the key exists in the dictionary, False otherwise.
 */
boolean key_exists(dict* dict, char* key);

/** Create a new dictionary.
 * 
 * This function creates a new, empty dictionary.
 *
 * @return A pointer to the newly created dictionary.
 */
dict* create_new_dict();

/** Create a new dictionary item.
 * 
 * This function creates a new dictionary item with the specified key.
 *
 * @param key The key associated with the dictionary item.
 * @return A pointer to the newly created dictionary item.
 */
dict_item* create_new_dict_item(char *key);

/** Add a dictionary item to the dictionary.
 * 
 * This function adds a dictionary item to the dictionary.
 *
 * @param dict The dictionary to which the dictionary item is to be added.
 * @param item The dictionary item to be added.
 */
void put_dict(dict* dict, dict_item* item);

/** Free the memory occupied by a dictionary and its items.
 * 
 * This function frees the memory occupied by a dictionary and its associated items.
 *
 * @param d The dictionary to be freed.
 */
void free_dict(dict* d);

/** Get the linked list of values associated with a key.
 * 
 * This function retrieves the linked list of values associated with a given key in the dictionary.
 *
 * @param dict The dictionary from which to retrieve the value.
 * @param key The key associated with the value to be retrieved.
 * @return A pointer to the linked list of values associated with the key, or NULL if the key is not found.
 */
linked_list* get_dict_value(dict* dict, char* key);

/** Create a new dictionary item with a key and value.
 * 
 * This function creates a new dictionary item with the specified key and linked list of values.
 *
 * @param key The key associated with the dictionary item.
 * @param value The linked list of values associated with the key.
 * @return A pointer to the newly created dictionary item.
 */
dict_item* new_dict_item(char* key, linked_list* value);

#endif