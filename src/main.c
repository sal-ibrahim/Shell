#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>



int main(int argc, char *argv[]) {
  //flush after every printf
  setbuf(stdout, NULL);

  char command[BUFFER_SIZE];

  //start autocomplete
  rl_attempted_completion_function = builtin_completion;

  //add space after completion
  rl_completion_append_character = ' ';

  while (1){
    char *line = readline("$ ");

    if (!line) {
        break;
    }

    if (*line) {
        add_history(line);
    }

    strncpy(command, line, BUFFER_SIZE - 1);
    command[BUFFER_SIZE - 1] = '\0';
    free(line);


    //handle exit command
    if (strcmp(command, "exit") == 0){
    return 0;
    }
    
    //handle pwd command
    if (handle_pwd(command)){
      continue;
    }

    //handle type command
    if (handle_type(command)){
    continue;
    }

    // handle cd command
    if (handle_cd(command)) {
      continue;
    }

    char *argv_exec[64];
    char command_copy[BUFFER_SIZE];

    strcpy(command_copy, command);
    parse_command(command_copy, argv_exec, 64);//split the command into argv_exec

    //only run if there is a program name
    if (argv_exec[0] != NULL) {
        run_external(argv_exec);
    }

  }
    return 0;
}

char *builtin_generator(const char *text, int state) {
    static int index;
    static int len;

    const char *builtins[] = { "echo", "exit", NULL };

    if (state == 0) {
        index = 0;
        len = strlen(text);
    }

    while (builtins[index]) {
        const char *name = builtins[index++];
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }

    return NULL;
}
char **builtin_completion(const char *text, int start, int end) {
    (void)start;
    (void)end;

    return rl_completion_matches(text, builtin_generator);
}
