/**
 * This file implements the 'type' command
 * 
 */


#include "builtins/builtin.h"
#include "path/path.h"
#include <stdio.h>

int builtin_type(int argc, char **argv) {
    // type requires an argument after 'type'
    if (argc < 2) {
        return SHELL_OK;
    }

    char *arg = argv[1];

    // Check if it's a builtin
    if (builtin_is_registered(arg)) {
        printf("%s is a shell builtin\n", arg);
        return SHELL_OK;
    }

    // Search PATH
    char full_path[BUFFER_SIZE];
    if (find_in_path(arg, full_path, sizeof(full_path))) {
        printf("%s is %s\n", arg, full_path);
        return SHELL_OK;
    }

    // Not found
    printf("%s: not found\n", arg);
    return SHELL_OK;
}
