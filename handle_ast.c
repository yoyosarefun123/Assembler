#include "handle_ast.h"
#include "bool.h"

#define MEMORY_IMAGE_START 100
typedef enum {LABEL_DATA, LABEL_ENT, LABEL_EXT, LABEL_CODE} type_label_table;
typedef enum {DIR_DATA, DIR_STRING, DIR_ENTRY, DIR_EXTERN} type_directive;
typedef enum {CONT_DATA, CONT_LAB} type_content;
typedef enum {LINE_ERR = -1, LINE_INST = 0, LINE_DIR = 1} type_line;

/*handles the ast built for each seperate line*/
void handle_ast(astNode* curr_line, linked_list** label_table, machine_code_line** code_img, machine_code_line** data_img, int* DC, int* IC, int line_cnt, int *error_cnt, dict* mcro_table) {
    /*get the type of line to know how to proceed*/
    int line_type = ast_get_line_type(curr_line);
    /*if line_type is equal to -1, there's an error and we immediately print it out finish running*/
    if (line_type == LINE_ERR) {
        char* err = ast_get_line_error(curr_line);
        printf("Error in line %d: %s\n", line_cnt, err);
        (*error_cnt)++;
        free(err);
        return;
    }
    /*if there is label, check that it is not a macro or other saved name*/
    if (ast_is_label(curr_line)) {
        char* lab = ast_get_line_label(curr_line);
        if (is_label_mcro(lab, mcro_table)) {
            printf("Error in line %d: label declaration is illegal as that name is saved for macro name.\n", line_cnt);
            (*error_cnt)++;
            free(lab);
            return;
        }
        free(lab);
    }
    /*if line_type is equal to 0, it's an instruction line and we handle it as such*/
    if (line_type == LINE_INST) {
        handle_instruction(curr_line, label_table, code_img, IC, line_cnt, error_cnt);
        return;
    }
    /*if line_type is equal to 1, it's a directive line and we handle it as such*/
    if (line_type == LINE_DIR) {
        handle_directive(curr_line, label_table, data_img, DC, line_cnt, error_cnt);
        return;
    }
}

/*if line is a directive line, parse it and insert the relevant values into label_table and data_img*/
void handle_directive(astNode* curr_line, linked_list** label_table, machine_code_line** data_img, int* DC, int line_cnt, int* error_cnt) {
    /*handle dir differently if it's .data, .string, .entry or .extern*/
    int dir_type = ast_dir_get_type(curr_line);
    /*check for errors in declaration of the dirctive*/
    /*if(error_directive(curr_line, dir_type));*/
    /*if type is .data, deal with it as such*/
    if (dir_type == DIR_DATA) {
        handle_data_dir(curr_line, label_table, data_img, DC, line_cnt, error_cnt);
    }
    /*if type is .string, deal with it as such*/
    if (dir_type == DIR_STRING) {
        handle_string_dir(curr_line, label_table, data_img, DC, line_cnt, error_cnt);
    }
    /*if type is .entry or .extern, deal with it as such*/
    if (dir_type == DIR_ENTRY) {
        handle_ent_dir(curr_line, label_table, DC, line_cnt, error_cnt);
    }
    if (dir_type == DIR_EXTERN) {
        handle_ext_dir(curr_line, label_table, DC, line_cnt, error_cnt);
    }
}

/*in general in this program, most of the errors are already taken care of in the ast and we do not deal 
with them in this c file, however they are not ignored and are printed in the original handle_ast function*/

/*if the directive sentence declares a data type label, check for errors and write it to the data segment image*/
void handle_data_dir(astNode* curr_line, linked_list** label_table, machine_code_line** data_img, int* DC, int line_cnt, int* error_cnt) {
    /*get the necessary data from the ast*/
    int amount_of_data = ast_dir_data_get_num_cnt(curr_line); 
    int* data;
    int i = 0;  
    /*check if a label is declared, if not just write the code to the data segment and if yes check for errors and add to label table*/
    if(ast_is_label(curr_line)) {
        char* lab = ast_get_line_label(curr_line);
        node* label;
        /*if it's already been previously defined, you cannot redefine it so return an error*/
        if(in_label_table(label_table, lab)) {
            printf("Error in line %d: label %s has already been declared and cannot be redefined.\n", line_cnt, lab);
            (*error_cnt)++;
            free(lab);
            return;
        }
        /*if it's already been declared as external, it cannot be defined in this file so return an error*/
        if (declared_extern(label_table[LABEL_EXT], lab)) {
            printf("Error in line %d: label %s has already been declared external and cannot be defined in this program.\n", line_cnt, lab);
            (*error_cnt)++;
            free(lab);
            return;
        }
        /*assuming there are no errors, add a node to the label table with the label as value and the DC + MEMORY_IMAGE_START as position*/
        label = create_node(lab);
        node_set_position(label, MEMORY_IMAGE_START + (*DC));
        add_node(label_table[LABEL_DATA], label);
        update_entry(label_table, lab);
        free(lab);
    }
    /*write the data to the data image*/
    data = ast_dir_data_get_num_array(curr_line);
    for (; i < amount_of_data; i++) {
        (*data_img + (*DC))->data_type = CONT_DATA;
        (*data_img + (*DC))->content.data = data[i];
        (*DC)++;
    }
    free(data);
 
}

/*this code updates the position of entry labels depending on when they're defined 
as code or data labels in the code or data segment*/
void update_entry(linked_list** label_table, char* label) {
    if (value_exists(label_table[LABEL_ENT], label) && value_exists(label_table[LABEL_DATA], label)) {
        update_position_by_value(label_table[LABEL_ENT], label, get_position_by_value(label_table[LABEL_DATA], label));
    }
    if (value_exists(label_table[LABEL_ENT], label) && value_exists(label_table[LABEL_CODE], label)) {
        update_position_by_value(label_table[LABEL_ENT], label, get_position_by_value(label_table[LABEL_CODE], label));
    }
}
/*these three functions are error checking functions that just make the code more concise, each check if 
a node with the value of a certain label already exists in different parts of the label table*/
boolean in_label_table(linked_list** label_table, char* label) {
    return (value_exists(label_table[LABEL_DATA], label) || value_exists(label_table[LABEL_CODE], label));
}

boolean declared_entry(linked_list* ent_list, char* label) {
    return value_exists(ent_list, label);
} 

boolean declared_extern(linked_list* ext_list, char* label) {
    return value_exists(ext_list, label);
} 

/*if the directive sentence declares a string, add it to the data segment 
and if necessary to the label table and check for errors*/
void handle_string_dir(astNode* curr_line, linked_list** label_table, machine_code_line** data_img, int* DC, int line_cnt, int* error_cnt) {
	/*get the necessary information from the ast*/
    char* string = ast_dir_get_string(curr_line);
	int i = 0;
    node* label;
    /*check if there is a label declaration, if there is then deal with it if not just write the string to the data segment*/
    if(ast_is_label(curr_line)) {
        char* lab = ast_get_line_label(curr_line);
        /*if this label has already been declared as data or code label earlier, return an error that it cannot be redefined*/
        if(in_label_table(label_table, lab)) {
            printf("Error in line %d: label %s has already been declared and cannot be redefined.\n", line_cnt, lab);
            (*error_cnt)++;
            free(lab);
            return;
        }
        /*if this label has already been declared external, we cannot define it in this code as it's defined somewhere else already*/
        if (declared_extern(label_table[LABEL_EXT], lab)) {
            printf("Error in line %d: label %s has already been declared external and cannot be defined in this program.\n", line_cnt, lab);
            (*error_cnt)++;
            free(lab);
            return;
        }
        /*if there are no errors, add it to the label_table with the position being the DC + MEMORY_IMAGE_START (since dc starts from 0 and not MEMORY_IMAGE_START)*/
        label = create_node(lab);
        node_set_position(label, MEMORY_IMAGE_START + (*DC));
        add_node(label_table[LABEL_DATA], label);
        update_entry(label_table, lab);
        free(lab);
    }
    for (; i < strlen(string); i++) {
        (*data_img + (*DC))->data_type = CONT_DATA;
        (*data_img + (*DC))->content.data = string[i];
        (*DC)++;
    }
    (*data_img + (*DC))->content.data = CONT_DATA;
    (*DC)++;
    free(string);

}
/*if the directive sentence declares an entry label, add it to the label table as entry and check for errors*/
void handle_ent_dir(astNode* curr_line, linked_list** label_table, int* DC, int line_cnt, int* error_cnt) {
    /*get the necessary information from the ast*/
    char** lab = ast_dir_get_labels(curr_line);
    int count = ast_dir_get_label_cnt(curr_line);
    int i = 0;
    node* label;
    /*loop through the labels in the input of .entry since it can have multiple labels*/
    for (;i < count; i++) {
        /*in this case, we don't check if the label already has a code or data definition since as 
        an entry it has to be both defined in the code/data segment and also be declared entry seperately*/
        /*if it's already been declared as an extern, return that it cannot be declared entry*/
        if (declared_extern(label_table[LABEL_EXT], lab[i])) {
            printf("Error in line %d: label %s has already been declared as external and cannot be declared entry.\n", line_cnt, lab[i]);
            (*error_cnt)++;
            free(lab[i]);
            continue;
        }
        /*if it's already been declared entry, return a warning that this line is unecessary*/
        if (declared_entry(label_table[LABEL_ENT], lab[i])) {
            printf("Warning in line %d: label %s has been already been previously declared entry.\n", line_cnt, lab[i]);
        }
        /*add to the label table a node with label as value and the position being -1 * line_cnt because we will later use that to check if 
        position has been updated or not, and be able to return the line in which the entry was originally declared */
        label = create_node(lab[i]);
        node_set_position(label, -line_cnt);
        add_node(label_table[LABEL_ENT], label);
        update_entry(label_table, lab[i]);
        free(lab[i]);
    }
    free(lab);
}
/*if the directive sentence declares an extern label, add it to the label table as external and check for errors*/
void handle_ext_dir(astNode* curr_line, linked_list** label_table, int* DC, int line_cnt, int* error_cnt) {
    /*get the necessary information from the ast*/
    char** lab = ast_dir_get_labels(curr_line);
    int count = ast_dir_get_label_cnt(curr_line);
    int i = 0;
    node* label;
    /*loop through the labels in the input of .extern since it can have multiple labels*/
    for (;i < count; i++) {
        /*if it's already defined as data or string or code, return an error as it cannot be an external label*/
        if (in_label_table(label_table, lab[i])) {
            printf("Error in line %d: label %s has already been declared in the code and cannot be declared external.\n", line_cnt, lab[i]);
            (*error_cnt)++;
            free(lab[i]);
            continue;
        }
        /*if it's already declared as entry, return an error as an entry cannot be external*/
        if (declared_entry(label_table[LABEL_ENT], lab[i])) {
            printf("Error in line %d: label %s has already been declared entry in the code and cannot be declared external.\n", line_cnt, lab[i]);
            (*error_cnt)++;
            free(lab[i]);
            continue;
        }
        /*if previously declared as extern already, just return a warning that this line is unecessary*/
        if (declared_extern(label_table[LABEL_EXT], lab[i])) {
            printf("Warning in line %d: label %s has been already been previously declared external.\n", line_cnt, lab[i]);
        }
        /*create a node with the label as its value and add to the table*/
        label = create_node(lab[i]);
        add_node(label_table[LABEL_EXT], label);
        free(lab[i]);
    }
    free(lab);
}

boolean is_label_mcro(char* label, dict* mcro_table) {
    if(key_exists(mcro_table, label)) return TRUE;
    return FALSE;
}

/*if line is an instruction line, parse it and insert the relevant values into label_table and code_img*/
void handle_instruction(astNode* curr_line, linked_list** label_table, machine_code_line** code_img, int* IC, int line_cnt, int* error_cnt) {
    /*handle instruction differently according to the number of operands in its argument*/
    int opcode = ast_inst_get_opcode(curr_line);
    
    /*check if there are any errors in the instruction itself (illegal parameters for example)*/
    if (error_instruction(curr_line, opcode, line_cnt, error_cnt)) return;
    /*check if there is a label declaration, if there is check for errors and add to label table*/
    if (ast_is_label(curr_line)) {
        char* lab = ast_get_line_label(curr_line);
        node* label;
        /*if this label has already been declared as data or code label earlier, return an error that it cannot be redefined*/
        if(in_label_table(label_table, lab)) {
            printf("Error in line %d: label %s has already been declared and cannot be redefined.\n", line_cnt, lab);
            (*error_cnt)++;
            free(lab);
            return;
        }
        /*if this label has already been declared external, we cannot define it in this code as it's defined somewhere else already*/
        if (declared_extern(label_table[LABEL_EXT], lab)) {
            printf("Error in line %d: label %s has already been declared external and cannot be defined in this program.\n", line_cnt, lab);
            (*error_cnt)++;
            free(lab);
            return;
        }
        label = create_node(lab);
        node_set_position(label, MEMORY_IMAGE_START + (*IC));
        add_node(label_table[LABEL_CODE], label);
        update_entry(label_table, lab);
        free(lab);
    }
   
    /*if opcode is between 0 and 4, it's a two operand instruction*/
    if (opcode <= 3 || opcode == 6) {
        handle_two_op_instruction(curr_line, code_img, IC);
    }
    /*if opcode is between 5 and 13, it's a one operand instruction*/
    else if (opcode <= 13) {
        handle_one_op_instruction(curr_line, code_img, IC);
    }
    /*if opcode is between 14 and 15 (which is what's left since ast_inst_get_opcode returns a value between 0 and 15), it's a zero operand instruction*/
    else {
        handle_zero_op_instruction(curr_line, code_img, IC);
    }
}

/*if instruction has two operands, handle it as such*/
void handle_two_op_instruction(astNode* curr_line, machine_code_line** code_img, int* IC) {
    /*if there are no errors (which was checked in earlier function), 
    start building the machine code that will be added to the code image*/
    int inst = 0;
    /*get the source and target operand type (immediate, label or register) with ast_inst_twoop_{}_addr()*/
    int src_addr = ast_inst_twoop_src_get_addr(curr_line);
    int trg_addr = ast_inst_twoop_trg_get_addr(curr_line);
    /*build the machine code for the instruction word according to operand types and opcode and add to code image*/
    inst += (src_addr << 9);
    inst += (trg_addr << 2);
    inst += (ast_inst_get_opcode(curr_line) << 5);
    (*code_img + (*IC))->data_type = CONT_DATA;
    (*code_img + (*IC))->content.data = inst;
    (*IC)++;
    /*add the source and target operand machine code, or the single word of machine code in the case of two registers*/
    if (src_addr == 5 && trg_addr == 5) {
        int reg = 0;
        reg += (ast_inst_twoop_src_get_reg(curr_line) << 7);
        reg += (ast_inst_twoop_trg_get_reg(curr_line) << 2);
        (*code_img + (*IC))->data_type = CONT_DATA;
        (*code_img + (*IC))->content.data = reg;
        (*IC)++;
    }
    else {
        add_twoop_src_value(curr_line, code_img, IC);
        add_twoop_trg_value(curr_line, code_img, IC);
    }
}

/*if instruction has one operand, handle it as such*/
void handle_one_op_instruction(astNode* curr_line, machine_code_line** code_img, int* IC) {
    /*if there are no errors (which was checked in earlier function), 
    start building the machine code that will be added to the code image*/
    int inst = 0;
    /*get target operand type*/
    int trg_addr = ast_inst_oneop_get_addr(curr_line);
    /*build the machine code for the instruction word according to operand type and opcode and add to code image*/
    inst += (trg_addr << 2);
    inst += (ast_inst_get_opcode(curr_line) << 5);
    (*code_img + (*IC))->data_type = CONT_DATA;
    (*code_img + (*IC))->content.data = inst;
    (*IC)++;
    /*add the target operand machine code to the code image*/
    add_oneop_trg_value(curr_line, code_img, IC);
}

/*if instruction has zero operands handle it as such*/
void handle_zero_op_instruction(astNode* curr_line, machine_code_line** code_img, int* IC) {
    (*code_img + (*IC))->data_type = CONT_DATA;
    (*code_img + (*IC))->content.data = ast_inst_get_opcode(curr_line) << 5;
    (*IC)++;
}

/*add the machine code for the source operand to the code image*/
void add_twoop_src_value(astNode* curr_line, machine_code_line** code_img, int* IC) {
    /*according to the type, add the number, the label name or the register number to code image*/
    if (ast_inst_twoop_src_get_addr(curr_line) == 1) {
        (*code_img + (*IC))->data_type = CONT_DATA;
        (*code_img + (*IC))->content.data = ast_inst_twoop_src_get_immed(curr_line) << 2;
        (*IC)++;
    }
    if (ast_inst_twoop_src_get_addr(curr_line) == 3) {
        char* lab = ast_inst_twoop_src_get_label(curr_line);
        (*code_img + (*IC))->data_type = CONT_LAB;
        strcpy((*code_img + (*IC))->content.label, lab);
        (*IC)++;
        free(lab);
    }
    if (ast_inst_twoop_src_get_addr(curr_line) == 5) {
        (*code_img + (*IC))->data_type = CONT_DATA;
        (*code_img + (*IC))->content.data = ast_inst_twoop_src_get_reg(curr_line) << 7;
        (*IC)++;
    }
}
/*add the machine code for the target operand to the code image (two operand instruction)*/
void add_twoop_trg_value(astNode* curr_line, machine_code_line** code_img, int* IC) {
    /*according to the type, add the number, the label name or the register number to code image*/
    if (ast_inst_twoop_trg_get_addr(curr_line) == 1) {
        (*code_img + (*IC))->data_type = CONT_DATA;
        (*code_img + (*IC))->content.data = ast_inst_twoop_trg_get_immed(curr_line) << 2;
        (*IC)++;
    }
    if (ast_inst_twoop_trg_get_addr(curr_line) == 3) {
        char* lab = ast_inst_twoop_trg_get_label(curr_line);
        (*code_img + (*IC))->data_type = CONT_LAB;
        strcpy((*code_img + (*IC))->content.label, lab);
        (*IC)++;
        free(lab);
    }
    if (ast_inst_twoop_trg_get_addr(curr_line) == 5) {
        (*code_img + (*IC))->data_type = CONT_DATA;
        (*code_img + (*IC))->content.data = ast_inst_twoop_trg_get_reg(curr_line) << 2;
        (*IC)++;
    }
}

/*add the machine code for the  target operand to the code image (one operand instruction)*/
void add_oneop_trg_value(astNode* curr_line, machine_code_line** code_img, int* IC) {
    if (ast_inst_oneop_get_addr(curr_line) == 1) {
        (*code_img + (*IC))->data_type = CONT_DATA;
        (*code_img + (*IC))->content.data = ast_inst_oneop_get_immed(curr_line) << 2;
        (*IC)++;
    }
    if (ast_inst_oneop_get_addr(curr_line) == 3) {
        char* lab = ast_inst_oneop_get_label(curr_line);
        strcpy((*code_img + (*IC))->content.label, lab);
        (*code_img + (*IC))->data_type = CONT_LAB;
        (*IC)++;
        free(lab);
    }
    if (ast_inst_oneop_get_addr(curr_line) == 5) {
        (*code_img + (*IC))->data_type = CONT_DATA;
        (*code_img + (*IC))->content.data = ast_inst_oneop_get_reg(curr_line) << 2;
        (*IC)++;
    }
}
/*check if there are errors in the instruction and if there are, print them out*/
boolean error_instruction(astNode* curr_line, int opcode, int line_cnt, int* error_cnt) {
    /*first check for lea as it's the only one i need to check for source operand*/
    if (opcode == 6) {
        if (ast_inst_twoop_src_get_addr(curr_line) == 1) {
            printf("Error in line %d: source operand is immed, should be label.\n", line_cnt);
            (*error_cnt)++;
            return TRUE;
        }
        if (ast_inst_twoop_src_get_addr(curr_line) == 5) {
            printf("Error in line %d: source operand is register, should be label\n", line_cnt);
            (*error_cnt)++;
            return TRUE;
        }
    }
    /*next check for all other instructions except cmp, prn, rts and stop 
    because for cmp and prn all target operands are legal, and for rts and stop we check for errors earlier
    when building the ast*/
    if (opcode != 1 && opcode != 12 && opcode != 14 && opcode != 15) {
        /*split into two since we have different functions for target operands in two operand and one operand instructions*/
        if (opcode <= 3 || opcode == 6) {
            if (ast_inst_twoop_trg_get_addr(curr_line) == 1) {
                printf("Error in line %d: target operand is immed, should be register or label\n", line_cnt);
                (*error_cnt)++;
                return TRUE;
            }
        }
        else {
            if (ast_inst_oneop_get_addr(curr_line) == 1) {
                printf("Error in line%d: target operand is immed, should be register or label\n", line_cnt);
                (*error_cnt)++;
                return TRUE;
            }
        }

    }
    return FALSE;
}
