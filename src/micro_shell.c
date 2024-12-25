/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_shell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ismherna <ismherna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:17:51 by ismherna          #+#    #+#             */
/*   Updated: 2024/12/03 12:31:59 by ismherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro_shell.h"
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Error handler to print messages and exit.
 */
void print_error(const char *msg) {
    write(2, msg, strlen(msg));
    exit(EXIT_FAILURE);
}

/**
 * Display a custom prompt.
 */
void display_prompt(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        strcpy(cwd, "unknown");
    }
    printf("\x1b[31mmicroshell\x1b[32m@:%s\x1b[37m$ ", cwd);
    fflush(stdout);
}

/**
 * Tokenize the input into arguments for execution.
 */
char **tokenize_input(const char *input) {
    size_t token_count = 0;
    char **tokens = NULL;
    char *copy = strdup(input);
    char *token = strtok(copy, " ");

    while (token) {
        tokens = realloc(tokens, sizeof(char *) * (token_count + 2));
        tokens[token_count++] = strdup(token);
        token = strtok(NULL, " ");
    }
    tokens[token_count] = NULL;
    free(copy);
    return tokens;
}

/**
 * Free tokenized arguments.
 */
void free_tokens(char **tokens) {
    for (size_t i = 0; tokens && tokens[i]; ++i) {
        free(tokens[i]);
    }
    free(tokens);
}

/**
 * Execute a command using execve.
 */
void execute_command(char **args, char **envp) {
    if (!args || !args[0]) return;

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "microshell: expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("microshell");
            }
        }
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) { // Child process
        char *path = getenv("PATH");
        char *full_path = NULL;
        char *dir = strtok(path, ":");
        while (dir) {
            full_path = malloc(strlen(dir) + strlen(args[0]) + 2);
            sprintf(full_path, "%s/%s", dir, args[0]);
            if (access(full_path, X_OK) == 0) {
                execve(full_path, args, envp);
                free(full_path);
                break;
            }
            free(full_path);
            dir = strtok(NULL, ":");
        }
        perror("execve");
        exit(EXIT_FAILURE);
    } else { // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

/**
 * Main shell loop.
 */
void shell_loop(char **envp) {
    char *line = NULL;

    while (1) {
        display_prompt();

        line = readline("");
        if (!line) {
            printf("\nExiting shell.\n");
            break;
        }

        // Add line to history
        if (*line) {
            add_history(line);
        }

        // Parse input and execute
        char **args = tokenize_input(line);
        execute_command(args, envp);

        free_tokens(args);
        free(line);
    }
}

int main(int argc, char **argv, char **envp) {
    (void)argc;
    (void)argv;

    printf("Welcome to Apollo's Microshell!\n");

    shell_loop(envp);

    printf("Shell terminated successfully.\n");
    return 0;
}
