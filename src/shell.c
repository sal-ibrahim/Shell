/*
 * 
 * This file contains the REPL Loop
 * It coordinates all subsystems: readline, parser, executor, history, completion.
 */

#include "shell.h"
#include "parser/parser.h"
#include "executor/executor.h"
#include "builtins/builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

//
extern char **builtin_completion(const char *text, int start, int end);
static void load_history(void);
static void save_history(void);

int shell_init(void) {
    setbuf(stdout, NULL);

    // Setup readline completion
    rl_attempted_completion_function = builtin_completion;
    rl_completion_append_character = ' ';

    // Load command history from file
    load_history();

    return SHELL_OK;
}

int shell_run(void) {
    char command[BUFFER_SIZE];

    while (1) {
        // Read command with readline
        char *line = readline("$ ");

        // EOF
        if (!line) {
            break;
        }

        // Add non-empty lines to history
        if (*line) {
            add_history(line);
        }

        strncpy(command, line, BUFFER_SIZE - 1);
        command[BUFFER_SIZE - 1] = '\0';
        free(line);

        // Handle exit command and save history
        if (strcmp(command, "exit") == 0) {
            save_history();
            return 0;
        }

        // Parse command into argument array
        char *argv[MAX_ARGS];
        char command_copy[BUFFER_SIZE];
        strcpy(command_copy, command);
        parse_command(command_copy, argv, MAX_ARGS);

        // Execute the command
        if (argv[0]) {
            execute_command(argv);
        }
    }

    return 0;
}

void shell_cleanup(void) {
    save_history();
}

// Load history
static void load_history(void) {
    char *histfile = getenv("HISTFILE");
    if (!histfile) {
        return;
    }

    FILE *fp = fopen(histfile, "r");
    if (!fp) {
        return;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), fp)) {
        // Strip newline
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines
        if (*line == '\0') {
            continue;
        }

        add_history(line);
    }

    fclose(fp);
}

// Save history to HISTFILE
static void save_history(void) {
    char *histfile = getenv("HISTFILE");
    if (!histfile) {
        return;
    }

    FILE *fp = fopen(histfile, "w");
    if (!fp) {
        return;
    }

    HIST_ENTRY **list = history_list();
    if (list) {
        for (int i = 0; list[i]; i++) {
            fprintf(fp, "%s\n", list[i]->line);
        }
    }

    fclose(fp);
}
