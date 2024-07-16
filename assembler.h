#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "bool.h"
#include "dictionary.h"
#include "linked_list.h"
#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "parsering_lines.h"
#include "handle_ast.h"

void assembler(char *argv);