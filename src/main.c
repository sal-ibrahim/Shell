/**
 * This file is the entry point that just initializes and runs the shell.
 */

#include "shell.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    /* Initialize shell */
    if (shell_init() != SHELL_OK) {
        fprintf(stderr, "Failed to initialize shell\n");
        return 1;
    }

    /* Run the REPL */
    int exit_code = shell_run();

    /* Cleanup */
    shell_cleanup();

    return exit_code;
}
