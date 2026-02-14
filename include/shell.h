#ifndef SHELL_H
#define SHELL_H

/*
 * 
 * This file defines the REPL loop and coordinates 
 * all shell subsystems (parser, executor, history, completion).
 */

#include "config.h"

/*
 * Initialize the shell and all subsystems
 * - Sets up readline completion
 * - Loads command history from HISTFILE
 * - Initializes builtin registry
 * 
 * Returns: SHELL_OK on success, SHELL_ERROR on failure
 */
int shell_init(void);

/*
 * Run the main REPL loop
 * - Displays prompt via readline
 * - Parses and executes commands
 * - Handles special commands (exit)
 * - Manages history
 * 
 * Returns: Exit status code
 */
int shell_run(void);

/*
 * Clean up shell resources and save state
 * - Saves command history to HISTFILE
 * - Frees allocated resources
 */
void shell_cleanup(void);

#endif
