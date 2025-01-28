#include <stdio.h>
#include "Main/interpreter.h"
#include "Test/test.h"
#include "Interactive/interactive.h"

int interactive_mode = 0;

int main(int argc, char **argv){

    if (argc == 3 && strcmp("./programa-principal",argv[0]) == 0) interpreter(argv[1], argv[2]);

    if (argc == 4 && strcmp("./programa-testes",argv[0]) == 0) test(argv[1], argv[2], argv[3]);

    if (argc == 1) {
        interactive_mode = 1;
        interactive();
    }
    
    return 0;
}