/**
 * This file implements the 'echo' command
 * 
 * 
 */

#include "builtins/builtin.h"
#include <stdio.h>

int builtin_echo(int argc, char **argv) {
    // Print all arguments after 'echo'
    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) {
            printf(" ");
        }
    }
    printf("\n");

    return SHELL_OK;
}
