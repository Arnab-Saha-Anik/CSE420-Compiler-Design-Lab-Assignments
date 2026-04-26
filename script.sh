#!/bin/bash

# First pass: Generate AST and symbol table
yacc -d -y --debug --verbose 22201299_22201291.y
echo 'Generated the parser C file and header file'
g++ -w -c -o y.o y.tab.c
echo 'Generated the parser object file'
flex 22201299_22201291.l
echo 'Generated the scanner C file'
g++ -fpermissive -w -c -o l.o lex.yy.c
echo 'Generated the scanner object file'
g++ y.o l.o -o two_pass_compiler
echo 'All ready, running the two-pass compiler...'

# Run the compiler on the input file
./two_pass_compiler input3.c
echo 'Compilation completed.'

# Display output files
echo '------------ Log output ------------'
cat 22201299_22201291_log.txt
echo '------------ Error output ------------'
cat 22201299_22201291_error.txt
echo '------------ Three Address Code ------------'
cat 22201299_22201291_code.txt