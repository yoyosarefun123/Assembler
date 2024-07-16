#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"

struct dict_item {
    char key[80];
    linked_list* value;
    struct dict_item* next;
};

struct dict {
    dict_item* head;
    dict_item* tail;
    int count;
};

void add_node_to_dict_item_list(struct dict_item *di, node *n)
{
	add_node(di->value, n);
}

boolean key_exists(struct dict* dict, char* key) {
    struct dict_item* current = dict->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}
struct dict* create_new_dict()
{
	struct dict *new = (struct dict*)malloc(sizeof(struct dict));
	new->head = NULL;
	new->tail = NULL;
	return new;
}
/* slightly misleading name, this just creates an empty dictionary item (the linked list is set to NULL) instead of also adding a value*/
struct dict_item* create_new_dict_item(char *key)
{
	struct dict_item *new = (struct dict_item*)malloc(sizeof(struct dict_item));
	new->value = create_new_list();
	strcpy(new->key, key);
	new->next = NULL;
	return new;
}
void put_dict(struct dict* dict, struct dict_item* item){
    struct dict_item *new_item = create_new_dict_item(item->key);
    linked_list *new_list;
    free(new_item->value);
    new_list = copy_list(item->value);
    new_item->value = new_list;
    
    /*free_list(item->value);*/
    free(item->value);
    free(item);
    
    if (key_exists(dict, new_item->key)) { 
        struct dict_item* current = dict->head;
        while (strcmp(current->key, new_item->key) != 0) {
            current = current->next;
        }
        current->value = new_item->value;
    }
    else {
        if (dict->head == NULL) {
            dict->head = new_item;
            dict->tail = new_item;
            dict->count = 1;
        }
        else {
            dict->tail->next = new_item;
            dict->tail = new_item;
            dict->count++;
        }
    }
}

void free_dict(struct dict* d) {
    struct dict_item* cur_item;
    if (d == NULL) return;

    cur_item = d->head;
    while (cur_item != NULL) {
        struct dict_item* next_item = cur_item->next;

        free_list(cur_item->value); 
        free(cur_item->value);
        free(cur_item);

        cur_item = next_item;
    }

    free(d);
}
linked_list* get_dict_value(struct dict* dict, char* key) {
    if (!key_exists(dict, key)){
        return NULL;
    }
    else {
        struct dict_item* current = dict->head;
        while (strcmp(current->key, key) != 0) {
            current = current->next;
        }
        return current->value;
    }
}

struct dict_item* new_dict_item(char* key, linked_list* value) {
    struct dict_item* item = (struct dict_item*)malloc(sizeof(struct dict_item));
    strcpy(item->key, key);
    item->value = value;
    return item;
}
