/**
 * This file implements part of the 'exit' command 
 * 
 */
#include "builtins/builtin.h"
#include <stdlib.h>

int builtin_exit(int argc, char **argv) {
    // Return code to indicate shell should exit 
    return SHELL_EXIT;
}
