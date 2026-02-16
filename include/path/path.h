#ifndef PATH_H
#define PATH_H

/**
 * This file defines functions for searching executables in PATH.
 * 
 * 
 */

#include <stddef.h>


/**
 * This function searches each directory in the PATH environment variable for an
 * executable file with the given name.
 * 
 * Parameters:
 *   cmd    - Command name to search for
 *   result - Buffer to store full path
 *   size   - Size of result buffer
 * 
 * Returns:
 *   1 if executable found 
 *   0 if not found
 * 
 */
int find_in_path(const char *cmd, char *result, size_t size);

#endif
