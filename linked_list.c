#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linked_list.h"
#include "bool.h"

/** Node structure to hold linked list elements */
struct node {
    struct node* next;
    char value[80];
    int position;
};

/** Linked list structure */
struct linked_list {
    struct node* head;
    struct node* tail;
    int count;
};

/** Creates a new node */
struct node* create_new_node() {
    struct node* new = (struct node*)malloc(sizeof(struct node));
    new->next = NULL;
    new->position = 0;
    return new;
}

/** Creates a new node with a specific value */
struct node* create_node(char *val) {
    struct node* new = (struct node*)malloc(sizeof(struct node));
    new->next = NULL;
    new->position = 0;
    strcpy(new->value, val);
    return new;
}

/** Sets the value of a node */
void node_set_value(struct node *n, char *new_val) {
    strcpy(n->value, new_val);
}

/** Sets the position of a node */
void node_set_position(struct node *n, int pos) {
    n->position = pos;
}

/** Creates a new linked list */
struct linked_list* create_new_list() {
    struct linked_list *new = (struct linked_list*)malloc(sizeof(struct linked_list));
    new->head = NULL;
    new->tail = NULL;
    new->count = 0;
    return new;
}

/** Copies a linked list */
struct linked_list* copy_list(struct linked_list *src) {
    struct linked_list *copy = create_new_list();
    struct node *cur = src->head;
    while(cur != NULL) {
        struct node *next = cur->next;
        add_node(copy, cur);
        cur = next;
    }
    return copy;
}

/** Adds a node to the linked list */
void add_node(struct linked_list* list, struct node* item) {
    struct node* entry;
    if (list == NULL) return;
    entry = (struct node*)malloc(sizeof(struct node));
    entry->next = item->next;
    strcpy(entry->value, item->value);
    entry->position = item->position;
    if (list->head == NULL) {
        list->head = entry;
        list->tail = entry;
        list->count++;
        free(item);
        return;
    }
    list->tail->next = entry;
    list->tail = list->tail->next;
    list->count++;
    free(item);
}

/** Returns the position of a node in the list */
int list_get_position (struct node * n) {
    return n->position;
}

/** Returns the length of the linked list */
int list_length(struct linked_list* list) {
    return list->count;
}

/** Prints the linked list to a file */
void print_list_to_file(struct linked_list *list, FILE *file) {
    struct node *cur = list->head;
    while (cur != NULL) {
        if (cur->position != 0) {
            fprintf(file, "%s   %d\n", cur->value, cur->position);
        } else {
            fprintf(file, "%s", cur->value);
        }
        cur = cur->next;
    }
}

/** Updates the position of data elements in the linked list */
void update_data_position(struct linked_list** label_table,  int* IC) {
    struct linked_list* data = label_table[0];
    struct node* cur;
    if (data == NULL) return;
    cur = data->head;
    while (cur != NULL) {
        int new_pos = cur->position + *IC;
        cur->position = new_pos;
        if (value_exists(label_table[1], cur->value)) {
            update_position_by_value(label_table[1], cur->value, cur->position);
        }
        cur = cur->next;
    }
}

/** Frees the memory used by the linked list */
void free_list(struct linked_list *list) {
    struct node *cur;
    if (list == NULL) return;
    cur = list->head;
    while (cur != NULL) {
        struct node *next = cur->next;
        free(cur);
        cur = next;
    }
}

/** Checks if a value exists in the linked list */
int value_exists(struct linked_list *list, char *word) {
    struct node *cur;
    if (list == NULL) return 0;
    cur = list->head;

    while (cur != NULL) {
        if (!strcmp(word, cur->value))
            return 1;
        cur = cur->next;
    }
    
    return 0;
}

/** Updates the position of a value in the linked list */
void update_position_by_value(struct linked_list* list, char* val, int pos) {
    struct node* cur;
    if (!value_exists(list, val) || list == NULL) return;
    cur = list->head;
    while (cur != NULL) {
        if (!strcmp(cur->value, val)) {
            cur->position = pos; 
        }
        cur = cur->next;
    }
}

/** Returns the position of a value in the linked list */
int get_position_by_value(struct linked_list* list, char* val) {
    struct node* cur;
    if (!value_exists(list, val) || list == NULL) return 0;
    cur = list->head;
    while (cur != NULL) {
        if (!strcmp(cur->value, val)) {
            return cur->position; 
        }
        cur = cur->next;
    }
    return 0;
}

/** Checks for undeclared entry labels */
boolean check_undeclared_ent(struct linked_list* ent, int* error_cnt) {
    struct node* cur = ent->head;
    boolean flag = TRUE;
    while (cur != NULL) {
        if (cur->position < 0) {
            printf("Error in line %d: label %s is declared as entry, but is not defined.\n", -1 * cur->position, cur->value);
            (*error_cnt)++;
            flag = FALSE;
        }
        cur = cur->next;
    }
    return flag;
}

/** Searches for a label in the linked list */
int search_label(char* label, struct linked_list* list) {
    struct node* cur;
    if (list == NULL) return -1;
    cur = list->head;
    while (cur != NULL) {
        if (!strcmp(cur->value, label)) return cur->position;
        cur = cur->next;
    }
    return -1;
}
