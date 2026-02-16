/**
 * 
 * This file implements command history
 */

#include "builtins/builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>

static int last_history_written = 0;

int builtin_history(int argc, char **argv) {
    /* history -r <file> - Read history from file */
    if (argc >= 3 && strcmp(argv[1], "-r") == 0) {
        char *path = argv[2];

        FILE *fp = fopen(path, "r");
        if (!fp) {
            perror("history");
            return SHELL_ERROR;
        }

        char line[BUFFER_SIZE];
        while (fgets(line, sizeof(line), fp)) {
            /* Strip newline */
            line[strcspn(line, "\n")] = '\0';

            /* Skip empty lines */
            if (*line == '\0') {
                continue;
            }

            add_history(line);
        }

        fclose(fp);
        return SHELL_OK;
    }

    /* history -w <file> - Write history to file */
    if (argc >= 3 && strcmp(argv[1], "-w") == 0) {
        char *path = argv[2];

        FILE *fp = fopen(path, "w");
        if (!fp) {
            perror("history");
            return SHELL_ERROR;
        }

        HIST_ENTRY **list = history_list();
        if (list) {
            for (int i = 0; list[i]; i++) {
                fprintf(fp, "%s\n", list[i]->line);
            }
        }

        fclose(fp);
        return SHELL_OK;
    }

    /* history -a <file> - Append new entries to file */
    if (argc >= 3 && strcmp(argv[1], "-a") == 0) {
        char *path = argv[2];

        FILE *fp = fopen(path, "a");
        if (!fp) {
            perror("history");
            return SHELL_ERROR;
        }

        HIST_ENTRY **list = history_list();
        if (list) {
            int i = last_history_written;
            while (list[i]) {
                fprintf(fp, "%s\n", list[i]->line);
                i++;
            }
            last_history_written = i;
        }

        fclose(fp);
        return SHELL_OK;
    }

    /* history [N] - Display history */
    int limit = -1;

    /* Parse optional number */
    if (argc >= 2) {
        limit = atoi(argv[1]);
        if (limit < 0) {
            limit = 0;
        }
    }

    HIST_ENTRY **list = history_list();
    if (!list) {
        return SHELL_OK;
    }

    /* Count total entries */
    int total = 0;
    while (list[total]) {
        total++;
    }

    /* Determine starting index */
    int start = 0;
    if (limit >= 0 && limit < total) {
        start = total - limit;
    }

    /* Display history */
    for (int i = start; i < total; i++) {
        printf("%5d  %s\n", i + 1, list[i]->line);
    }

    return SHELL_OK;
}
