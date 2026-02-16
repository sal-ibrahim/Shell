#ifndef COMPLETION_H
#define COMPLETION_H


/**
 * This file defines the command completion
 */


 /**
  * 
  * Parameters:
  *  text  - Text/command to complete
  *  start - Starting position in line buffer
  *  end   - Ending position in line buffer
  * 
  * Returns:
  * Array of possible completions, or NULL
  * 
  */
char **builtin_completion(const char *text, int start, int end);

#endif

