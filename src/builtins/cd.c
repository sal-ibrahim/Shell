/**
 * 
 * This file implements the 'cd' command
 */

#include "builtins/builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int builtin_cd(int argc, char **argv) {
    // cd with no arguments
    if (argc < 2) {
        return SHELL_OK;
    }

    char *path = argv[1];

    if (strcmp(path, "~") == 0) {
        path = getenv("HOME");
        if (!path) {
            fprintf(stderr, "cd: HOME not set\n");
            return SHELL_ERROR;
        }
    }

    // Check if directory exists
    struct stat st;
    if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "cd: %s: No such file or directory\n", path);
        return SHELL_ERROR;
    }

    // Attempt to change directory
    if (chdir(path) != 0) {
        fprintf(stderr, "cd: %s: No such file or directory\n", path);
        return SHELL_ERROR;
    }

    return SHELL_OK;
}
