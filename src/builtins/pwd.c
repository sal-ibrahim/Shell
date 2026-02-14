/**
 * 
 * This file implements the 'pwd' command
 */

#include "builtins/builtin.h"
#include <stdio.h>
#include <unistd.h>

int builtin_pwd(int argc, char **argv) {
    
    char buf[BUFFER_SIZE];

    if (getcwd(buf, sizeof(buf)) != NULL) {
        printf("%s\n", buf);
        return SHELL_OK;
    }

    perror("pwd");
    return SHELL_ERROR;
}
