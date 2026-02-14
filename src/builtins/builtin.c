/**
 * This file handles the builtin commands using a function pointer
 * table pattern.
 * 
 */

#include "builtins/builtin.h"
#include <string.h>
#include <stdio.h>

/* All builtin commands */
static BuiltinCommand builtins[] = {
    {"exit",    builtin_exit},
    {"echo",    builtin_echo},
    {"pwd",     builtin_pwd},
    {"cd",      builtin_cd},
    {"type",    builtin_type},
    {"history", builtin_history},
    {NULL,      NULL}
};


int builtin_is_registered(const char *name) {
    if (!name) {
        return 0;
    }

    for (int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(name, builtins[i].name) == 0) {
            return 1;
        }
    }

    return 0;
}

int builtin_execute(const char *name, int argc, char **argv) {
    if (!name) {
        return SHELL_ERROR;
    }

    // Find and execute the builtin
    for (int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(name, builtins[i].name) == 0) {
            return builtins[i].handler(argc, argv);
        }
    }

    // Not found just in case of error
    fprintf(stderr, "%s: builtin not found\n", name);
    return SHELL_ERROR;
}

const char **builtin_get_names(void) {
    // Return pointer to static array of names (for completion)
    static const char *names[64];
    static int initialized = 0;

    if (!initialized) {
        int i;
        for (i = 0; builtins[i].name != NULL && i < 63; i++) {
            names[i] = builtins[i].name;
        }
        names[i] = NULL;
        initialized = 1;
    }

    return names;
}
