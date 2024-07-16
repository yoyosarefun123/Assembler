#include <string.h>
#include "linked_list.h"
#include "parsering_lines.h"
#include "base_64.h"
#include "bool.h"
#include "handle_ast.h"
#include "second_pass.h"

#define TYPE_LABEL 1
#define MEMORY_IMAGE_START 100
typedef enum {DATA, ENTRY, EXTERN, CODE} label_table_type;
#define EXT_RETURN 1
#define RELOC 2

void second_pass(char* file_name, machine_code_line** code_img, machine_code_line** data_img, linked_list** label_table, linked_list* ext_img, int* DC, int* IC, int* error_cnt) {
    int i = 0;
    char* file_ob_name = add_file_type(file_name, ".ob"); 
    FILE *ob = fopen(file_ob_name, "w+");
    fprintf(ob, "%d    %d\n", *IC, *DC); 

    /* Iterate through the code image to generate the object file */
    for(; i < *IC; i++) {
        char* base_64;

        /* If the data type is a label, convert the label to machine code */
        if ((*code_img + i)->data_type == TYPE_LABEL) {
            char* label = malloc(31 * sizeof(char));
            int code;
            strcpy(label, (*code_img + i)->content.label);
            code = label_to_code(label, label_table, ext_img, i + MEMORY_IMAGE_START);
            if (code == -1) /*print error*/ {
                (*error_cnt)++;
                printf("Error: label %s is undefined\n", label);
            }
            (*code_img + i)->content.data = code;
            free(label);
        }

        /* Convert the machine code to base 64 and write to the object file */
        base_64 = decimal_to_base64((*code_img + i)->content.data);
        fprintf(ob, "%s\n", base_64);
        free(base_64);
    } 

    /* Iterate through the data image to generate the object file */
    i = 0;
    for(; i < *DC; i++) {
        char* base_64 = decimal_to_base64((*data_img + i)->content.data);
        fprintf(ob, "%s\n", base_64);
        free(base_64);
    }

    /* Check undeclared entry labels and generate entry and extern files */
    if (check_undeclared_ent(label_table[ENTRY], error_cnt)) {
        if (list_length(label_table[ENTRY])) {
            char* file_ent_name = add_file_type(file_name, ".ent");
            FILE *ent = fopen(file_ent_name, "w+");
            print_list_to_file(label_table[ENTRY], ent);
            fclose(ent);
            free(file_ent_name);
        }
    }

    /* Print error message and remove object file in case of errors */
    if((*error_cnt)) {
        printf("There are %d errors in your program after second run\n", *error_cnt);
        printf("filename is: %s", file_ob_name);
        fclose(ob);
        free(file_ob_name);
        return;
    }

    if (list_length(ext_img)) {
        char* file_ext_name = add_file_type(file_name, ".ext");
        FILE *ext = fopen(file_ext_name, "w+");
        print_list_to_file(ext_img, ext);
        fclose(ext);
        free(file_ext_name);
    }
    fclose(ob);
    free(file_ob_name);
    
}


int label_to_code(char* label, linked_list** label_table, linked_list* ext_img, int pos) {
    linked_list* label_data = label_table[DATA];
    linked_list* label_ext = label_table[EXTERN];
    linked_list* label_code = label_table[CODE];
    int code = 0;

    /* Check if the label is an external symbol and add it to the ext_img list */
    if (search_label(label, label_ext) != -1) {
        node* new = create_node(label);
        node_set_position(new, pos);
        add_node(ext_img, new);
        return EXT_RETURN;
    }
    else {
        /* Check if the label is in the data or code segments and assign the appropriate code */
        if ((code = search_label(label, label_data)) != -1) {
            return RELOC + (code << 2);
        }
        if ((code = search_label(label, label_code)) != -1) {
            return RELOC + (code << 2);
        }
    }
    return -1;
}
