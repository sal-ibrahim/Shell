/**
 * This file implements the command parsing logic
 */

#include "parser/parser.h"
#include <string.h>

int parse_command(char *input, char **argv, int max_args) {
    int argc = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    char *p = input;
    char *arg_start = NULL;

    while (*p) {
        // Skip leading spaces
        while (*p == ' ' && !in_single_quotes && !in_double_quotes) {
            p++;
        }

        if (*p == '\0') {
            break;
        }

        // Mark the start of the argument
        arg_start = p;
        argv[argc++] = p;

        while (*p) {
            // Handle single quotes
            if (*p == '\'' && !in_double_quotes) {
                in_single_quotes = !in_single_quotes;
                // Remove quote by shifting
                memmove(p, p + 1, strlen(p));
                continue;
            }

            // Handle double quotes
            if (*p == '"' && !in_single_quotes) {
                in_double_quotes = !in_double_quotes;
                // Remove quote by shifting
                memmove(p, p + 1, strlen(p));
                continue;
            }

            // Handle backslash outside quotes
            if (*p == '\\' && !in_single_quotes && !in_double_quotes) {
                // Remove \ by shifting
                memmove(p, p + 1, strlen(p));
                if (*p == '\0') {
                    break;
                }
                p++;
                continue;
            }

            /* If space outside of quotes, the argument ends */
            if (*p == ' ' && !in_single_quotes && !in_double_quotes) {
                /* Null-terminate the current argument and move past the space */
                *p = '\0';
                p++;
                break;
            }

            // Handle backslashes inside quotes
            if (*p == '\\') {
                if (in_double_quotes) {
                    // In double quotes, only \ and " are special
                    char next = *(p + 1);
                    if (next == '"' || next == '\\') {
                        //Shift left to remove backslash
                        memmove(p, p + 1, strlen(p));
                        // Move past the escaped character
                        p++;
                        continue;
                    }
                    p++;
                    continue;
                } else if (!in_single_quotes) {
                    memmove(p, p + 1, strlen(p));
                    if (*p == '\0') {
                        break;
                    }
                    p++;
                    continue;
                }
            }

            // Move to the next character
            p++;
        }
        if (argc >= max_args - 1) {
            break;
        }
    }

    argv[argc] = NULL;
    return argc;
}

int has_pipeline(char **argv, int *pipe_index) {
    if (!argv || !pipe_index) {
        return 0;
    }

    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0) {
            *pipe_index = i;
            return 1;
        }
    }

    return 0;
}
