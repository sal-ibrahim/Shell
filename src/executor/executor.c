/*
 * 
 * This file handles command execution etiher builtin, external, or pipeline
 */

#include "executor/executor.h"
#include "builtins/builtin.h"
#include "parser/parser.h"
#include "path/path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

static int parse_redirects(char **argv, char **out_file, char **err_file, int *append_out, int *append_err);
static int setup_redirects(char *out_file, char *err_file, int append_out, int append_err);

int execute_command(char **argv) {
    if (!argv || !argv[0]) {
        return SHELL_OK;
    }

    if (builtin_is_registered(argv[0])) {
        int argc = 0;
        while (argv[argc] != NULL) {
            argc++;
        }
        return builtin_execute(argv[0], argc, argv);
    }

    // Check for pipeline
    int pipe_index;
    if (has_pipeline(argv, &pipe_index)) {
        argv[pipe_index] = NULL;

        // Split command
        char **left = argv;
        char **right = &argv[pipe_index + 1];
        return execute_pipeline(left, right);
    }

    // Execute external command
    return execute_external(argv);
}

int execute_external(char **argv) {
    char full_path[BUFFER_SIZE];
    char *out_file = NULL;
    char *err_file = NULL;
    int append_out = 0;
    int append_err = 0;

    // Parse and remove redirections from argv
    if (parse_redirects(argv, &out_file, &err_file, &append_out, &append_err) != 0) {
        return SHELL_ERROR;
    }

    // Find executable in PATH
    if (!find_in_path(argv[0], full_path, sizeof(full_path))) {
        printf("%s: command not found\n", argv[0]);
        return SHELL_ERROR;
    }

    // Fork child process
    pid_t pid = fork();

    if (pid == 0) {        
        // Setup redirections
        if (setup_redirects(out_file, err_file, append_out, append_err) != 0) {
            exit(1);
        }

        // Replace child process with program
        execv(full_path, argv);
        perror("execv");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
        return SHELL_OK;
    } else {
        perror("fork");
        return SHELL_ERROR;
    }
}

int execute_pipeline(char **left, char **right) {
    int fd[2];
    
    if (pipe(fd) == -1) {
        perror("pipe");
        return SHELL_ERROR;
    }

    // Fork first command
    pid_t pid1 = fork();
    if (pid1 == 0) {
      // Write
      dup2(fd[1], STDOUT_FILENO);
      close(fd[0]);
      close(fd[1]);

      char path[BUFFER_SIZE];
      if (!find_in_path(left[0], path, sizeof(path))) {
          fprintf(stderr, "%s: command not found\n", left[0]);
          exit(1);
      }

      execv(path, left);
      perror("execv");
      exit(1);
    }

    // Fork second command
    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Read
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);

        char path[BUFFER_SIZE];
        if (!find_in_path(right[0], path, sizeof(path))) {
            fprintf(stderr, "%s: command not found\n", right[0]);
            exit(1);
        }

        execv(path, right);
        perror("execv");
        exit(1);
    }

    // Close parent, end pipe, and wait
    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return SHELL_OK;
}

static int parse_redirects(char **argv, char **out_file, char **err_file, int *append_out, int *append_err) {
    for (int i = 0; argv[i] != NULL; i++) {
        // Check for >> or 1>>
        if (strcmp(argv[i], ">>") == 0 || strcmp(argv[i], "1>>") == 0) {
            if (argv[i + 1] == NULL) {
                fprintf(stderr, "syntax error: expected file after >>\n");
                return -1;
            }
            *out_file = argv[i + 1];
            *append_out = 1;
            argv[i] = NULL;
            break;
        }

        // Check for > or 1>
        if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], "1>") == 0) {
            if (argv[i + 1] == NULL) {
                fprintf(stderr, "syntax error: expected file after >\n");
                return -1;
            }
            *out_file = argv[i + 1];
            *append_out = 0;
            argv[i] = NULL;
            break;
        }

        // Check for 2>>
        if (strcmp(argv[i], "2>>") == 0) {
            if (argv[i + 1] == NULL) {
                fprintf(stderr, "syntax error: expected file after 2>>\n");
                return -1;
            }
            *err_file = argv[i + 1];
            *append_err = 1;
            argv[i] = NULL;
            break;
        }

        // Check for 2>
        if (strcmp(argv[i], "2>") == 0) {
            if (argv[i + 1] == NULL) {
                fprintf(stderr, "syntax error: expected file after 2>\n");
                return -1;
            }
            *err_file = argv[i + 1];
            *append_err = 0;
            argv[i] = NULL;
            break;
        }
    }

    return 0;
}

static int setup_redirects(char *out_file, char *err_file, int append_out, int append_err) {
    // Setup stdout redirection
    if (out_file) {
        int flags = O_WRONLY | O_CREAT;
        flags |= append_out ? O_APPEND : O_TRUNC;

        int fd = open(out_file, flags, 0644);
        if (fd < 0) {
            perror("open");
            return -1;
        }

        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    // Setup stderr redirection
    if (err_file) {
        int flags = O_WRONLY | O_CREAT;
        flags |= append_err ? O_APPEND : O_TRUNC;

        int fd = open(err_file, flags, 0644);
        if (fd < 0) {
            perror("open");
            return -1;
        }

        dup2(fd, STDERR_FILENO);
        close(fd);
    }

    return 0;
}
