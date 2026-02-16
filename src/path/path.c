/**
 * 
 * This file provides searches executables in PATH
 */

#include "path/path.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int find_in_path(const char *cmd, char *result, size_t size) {
    // Get PATH
    char *path = getenv("PATH");
    if (path == NULL) {
        return 0;
    }

    char path_copy[BUFFER_SIZE];
    strncpy(path_copy, path, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';

    // Tokenize PATH by on : 
    char *dir = strtok(path_copy, ":");

    while (dir != NULL) {
        // Build full path
        char full_path[BUFFER_SIZE];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        // Check if file exists and is executable
        if (access(full_path, X_OK) == 0) {
            strncpy(result, full_path, size);
            result[size - 1] = '\0';
            return 1;
        }

        // Get next directory in PATH
        dir = strtok(NULL, ":");
    }

    return 0;
}
