#ifndef CONFIG_H
#define CONFIG_H

/*
 * This file contains all shared constants, macros, and types used throughout.
 *
 */

#include <stddef.h>

/* Buffer size for command input */
#define BUFFER_SIZE 1024

/* Maximum number of arguments in a command */
#define MAX_ARGS 64

/* Maximum number of PATH directories */
#define MAX_PATH_DIRS 128

/* Return status codes */
typedef enum {
    SHELL_OK = 0,           /* Success */
    SHELL_ERROR = 1,        /* General error */
    SHELL_EXIT = 2,         /* User requested exit */
    SHELL_PARSE_ERROR = 3,  /* Command parsing failed */
    SHELL_EXEC_ERROR = 4    /* Command execution failed */
} ShellStatus;

/* Command structure for parsed commands */
typedef struct {
    char **argv;            /* Null-terminated argument array */
    int argc;               /* Argument count */
    char *output_file;      /* stdout redirect target (NULL if none) */
    char *error_file;       /* stderr redirect target (NULL if none) */
    int append_output;      
    int append_error;       
} Command;

#endif
