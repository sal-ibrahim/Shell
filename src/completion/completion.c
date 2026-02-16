/**
 * 
 * This file contains the autocompletion for commands
 */

#include "completion/completion.h"
#include "builtins/builtin.h"
#include "path/path.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <readline/readline.h>

static char *builtin_generator(const char *text, int state);
static char *executable_generator(const char *text, int state);


char **builtin_completion(const char *text, int start, int end) {

    // complete at start of line
    if (start != 0) {
        return NULL;
    }

    char **matches = NULL;

    // Get builtin matches
    matches = rl_completion_matches(text, builtin_generator);
    
    // Get executable matches
    char **execs = rl_completion_matches(text, executable_generator);

    // Only one type of matches
    if (!matches) return execs;
    if (!execs) return matches;

    // Merge both lists
    int i, j;
    for (i = 0; matches[i]; i++);
    for (j = 0; execs[j]; j++);

    matches = realloc(matches, (i + j + 1) * sizeof(char *));
    for (int k = 0; k < j; k++) {
        matches[i + k] = execs[k];
    }
    matches[i + j] = NULL;

    free(execs);
    return matches;
}


static char *builtin_generator(const char *text, int state) {
    static int index;
    static int len;

    if (state == 0) {
        index = 0;
        len = strlen(text);
    }

    const char **builtins = builtin_get_names();

    // Find next matching builtin
    while (builtins[index]) {
        const char *name = builtins[index++];
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }

    return NULL;
}

static char *executable_generator(const char *text, int state) {
    static char **paths = NULL;
    static int path_index;
    static DIR *dir;
    static struct dirent *entry;
    static int text_len;

    if (state == 0) {
        text_len = strlen(text);
        path_index = 0;

        // Parse PATH into array
        char *path_env = getenv("PATH");
        if (!path_env) return NULL;

        static char path_copy[BUFFER_SIZE];
        strncpy(path_copy, path_env, sizeof(path_copy));
        path_copy[sizeof(path_copy) - 1] = '\0';

        static char *path_array[MAX_PATH_DIRS];
        int i = 0;
        char *token = strtok(path_copy, ":");
        while (token && i < MAX_PATH_DIRS - 1) {
            path_array[i++] = token;
            token = strtok(NULL, ":");
        }
        path_array[i] = NULL;
        paths = path_array;
        dir = NULL;
    }

    // Iterate through PATH directories
    while (1) {
        if (!dir) {
            if (!paths[path_index]) return NULL;

            dir = opendir(paths[path_index++]);
            if (!dir) {
                continue;
            }
        }

        // Read directory entries
        while ((entry = readdir(dir)) != NULL) {
            if (strncmp(entry->d_name, text, text_len) == 0) {
                // Build full path
                char full_path[BUFFER_SIZE];
                snprintf(full_path, sizeof(full_path),
                        "%s/%s", paths[path_index - 1], entry->d_name);

                // Check if path is executable
                if (access(full_path, X_OK) == 0) {
                    return strdup(entry->d_name);
                }
            }
        }

        closedir(dir);
        dir = NULL;
    }
}
