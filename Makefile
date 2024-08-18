CC := gcc
CFLAGS := -ansi -c -Wall -pedantic

assembler: main.o assembler.o ast.o dictionary.o linked_list.o parsering_lines.o saved_names.o pre_assembler.o first_pass.o second_pass.o handle_ast.o base_64.o
	$(CC) -g main.o assembler.o dictionary.o linked_list.o parsering_lines.o saved_names.o ast.o pre_assembler.o first_pass.o second_pass.o handle_ast.o base_64.o -o assembler
main.o: main.c assembler.h 
	$(CC) $(CFLAGS) main.c -o main.o
assembler.o: assembler.c assembler.h ast.h bool.h dictionary.h linked_list.h pre_assembler.h first_pass.h second_pass.h handle_ast.h
	$(CC) $(CFLAGS) assembler.c -o assembler.o
dictionary.o: dictionary.c dictionary.h bool.h linked_list.h
	$(CC) $(CFLAGS) dictionary.c -o dictionary.o
linked_list.o: linked_list.c linked_list.h bool.h
	$(CC) $(CFLAGS) linked_list.c -o linked_list.o
pre_assembler.o: pre_assembler.c pre_assembler.h parsering_lines.h linked_list.h dictionary.h saved_names.h bool.h 
	 $(CC) $(CFLAGS) pre_assembler.c -o pre_assembler.o
parsering_lines.o: parsering_lines.c parsering_lines.h ast.h
	$(CC) $(CFLAGS) parsering_lines.c -o parsering_lines.o
saved_names.o: saved_names.c saved_names.h
	$(CC) $(CFLAGS) saved_names.c -o saved_names.o
ast.o: ast.c ast.h bool.h linked_list.h dictionary.h saved_names.h parsering_lines.h
	$(CC) $(CFLAGS) ast.c -o ast.o
first_pass.o: first_pass.c first_pass.h handle_ast.h linked_list.h
	$(CC) $(CFLAGS) first_pass.c -o first_pass.o
second_pass.o: second_pass.c second_pass.h linked_list.h parsering_lines.h parsering_lines.h base_64.h bool.h
	$(CC) $(CFLAGS) second_pass.c -o second_pass.o
handle_ast.o: handle_ast.c handle_ast.h ast.h bool.h linked_list.h 
	$(CC) $(CFLAGS) handle_ast.c -o handle_ast.o
base_64.o: base_64.c base_64.h 
	$(CC) $(CFLAGS) base_64.c -o base_64.o
clean: 
	rm *.o
