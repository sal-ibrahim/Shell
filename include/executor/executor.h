#ifndef EXECUTOR_H
#define EXECUTOR_H

/**
 * This file defines the command execution methods:
 *
 * 
 */
#include "config.h"

/**
 * Start for command execution
 * 
 * This function:
 * - Checks if command is builtin
 * - Detects pipelines
 * - Delegates to appropriate executor
 * 
 * Parameters:
 *   argv - Parsed argument array
 * 
 * Returns:
 *   SHELL_OK on success
 *   SHELL_ERROR on failure
 */
int execute_command(char **argv);


/**
 * Execute an external program
 * 
 * This function:
 * - Searches PATH for executable
 * - Parses I/O redirections (>, >>, 2>, 2>>)
 * - Forks child process
 * - Sets up redirections
 * - Executes program
 * - Waits for completion
 * 
 * Parameters:
 *   argv - Argument array
 * 
 * Returns:
 *   SHELL_OK on success
 *   SHELL_ERROR if command not found or execution failed
 */
int execute_external(char **argv);


/**
 * Execute a pipeline of two commands
 * 
 * This function creates pipe, forks two processes, connects them, and waits for both.
 * 
 * Parameters:
 *   left  - First command (stdout)
 *   right - Second command (stdin)
 * 
 * Returns:
 *   SHELL_OK on success
 *   SHELL_ERROR on failure
 */
int execute_pipeline(char **left, char **right);

#endif
