#ifndef PARSER_H
#define PARSER_H


/**
 * This file defines the command parsing
 */

#include "config.h"

/**
 * Parse a command string into argument array
 * 
 * This function:
 * - Splits on whitespace outside quotes
 * - Removes quotes from arguments
 * - Processes backslash escapes
 * - Preserves spaces inside quotes
 * - Null-terminates each argument
 * 
 * 
 * Parameters:
 *   input - Command string (will be modified)
 *   argv  - Output array for argument pointers
 *   max_args - Maximum number of arguments 
 * 
 * Returns:
 *   Number of arguments parsed
 */
int parse_command(char *input, char **argv, int max_args);


/**
 * Detect pipeline operator in the command
 * 
 * Parameters:
 *   argv       - Parsed argument array
 *   pipe_index - Output: index of pipe operator (if found)
 * 
 * Returns:
 *   1 if pipe found, 0 otherwise
 */
int has_pipeline(char **argv, int *pipe_index);

#endif

