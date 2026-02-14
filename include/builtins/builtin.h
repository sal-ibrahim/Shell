#ifndef BUILTIN_H
#define BUILTIN_H

/**
 * This file provides the registry for all shell builtin commands.
 * Builtins are registered in a table of function pointers.
 * 
 * - Builtins are registered once at initialization
 * - Each builtin function receives argc/argv
 */

#include "config.h"

/**
 * Builtin function and command entry
 * 
 * - builtin_func_t is a pointer to a function that takes (int, char**) and returns int")
 * 
 */
typedef int (*builtin_func_t)(int argc, char **argv);

typedef struct {
    const char *name;           // Command name
    builtin_func_t handler;     // Function pointer to handler
} BuiltinCommand;

/**
 * Check if a command is a registered builtin
 * 
 * Parameters:
 *   name - Command name to check
 * 
 * Returns:
 *   1 if builtin exists, 0 otherwise
 */
int builtin_is_registered(const char *name);


/**
 * Execute a builtin command
 * 
 * Parameters:
 *   name - Command name
 *   argc - Argument count
 *   argv - Argument array
 * 
 * Returns:
 *   Return value from builtin handler, or SHELL_ERROR if not found 
 */
int builtin_execute(const char *name, int argc, char **argv);


/**
 * Get array of all builtin names (for completion)
 * 
 * Returns:
 *   Null-terminated array of builtin names
 */
const char **builtin_get_names(void);

/* Individual builtins */
int builtin_exit(int argc, char **argv);
int builtin_echo(int argc, char **argv);
int builtin_pwd(int argc, char **argv);
int builtin_cd(int argc, char **argv);
int builtin_type(int argc, char **argv);
int builtin_history(int argc, char **argv);

#endif
