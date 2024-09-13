# Assembler Project

Hello! This is Daniel and Yoav's assembler project.

We built it in such a fashion that if you do not enter any files, you get a short explanation about the project.

The project in general takes assembly code (from a non-existent assembly language created for the sake of this university course) and translates it into 3 files: 
  1. .ob file which holds the memory image, which is basically all the binary (encoded in base64) instructions that the assembly program creates.
  2. .ent file which shows where each .entry label was defined in our program.
  3. .ext file which shows where each .extern label was referred to in the assembly code.

The way we do this is in three (general) stages: 
  1. Pre-assembler: take the original .as code and parse it, removing things such as comments and empty lines.
  2. First pass: We go through each line of the .am output file from the pre-assembler, and then we do two things:

     2.1. Building an ast: we parse the .am line using an abstract syntax tree, each line gets a completely seperate tree.

     2.2 Handling the ast and updating values: we take the information from the ast, and compute the values that need to go in the data image, the code image and the label table.
  
  3. Second pass: After the first pass is done, we just go over the code image and exchange all labels with the location of the label they refer to - and then we create the files as we explained before.

In the example runs we have provided two examples of the code working properly (example 2 and example 4) and two examples of the code running into errors (once in the first pass and once in the second pass, as we do not move on to the next pass if there are errors in the previous pass). That's why example 2 and 4 have their own output files, and example 1 and 3 have a .txt file which holds all the errors/warnings printed to the terminal for the user's enjoyment.

Thank you for taking a look at our project!
