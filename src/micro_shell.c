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

#include "../includes/micro_shell.h"

// Forward declaration of cat_command
void cat_command(char **cmd);

// Funciones de utilidad
void err(const char *msg) {
    while (*msg)
        write(2, msg++, 1);
}

void add_to_history(const char *cmd) {
    t_history *new_entry = malloc(sizeof(t_history));
    if (!new_entry)
        return err("error: fatal\n"), exit(1);
    new_entry->command = strdup(cmd);
    new_entry->next = g_history;
    g_history = new_entry;
}

void print_history() {
    t_history *current = g_history;
    while (current) {
        printf("%s\n", current->command);
        current = current->next;
    }
}

// Liberación del historial
void free_history() {
    t_history *current = g_history;
    while (current) {
        t_history *next = current->next;
        free(current->command);
        free(current);
        current = next;
    }
}

// Creación de nodos AST
t_ast_node *ast_create_node(char **cmd) {
    t_ast_node *node = malloc(sizeof(t_ast_node));
    if (!node)
        return err("error: fatal\n"), exit(1), NULL;
    node->cmd = cmd;
    node->heredoc = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Liberación del AST
void ast_free(t_ast_node *root) {
    if (!root) return;
    ast_free(root->left);
    ast_free(root->right);
    free(root->heredoc);
    free(root);
}

// Manejo de Heredoc
char *handle_heredoc(const char *delimiter) {
    char *line = NULL;
    size_t len = 0;
    char *result = strdup("");

    printf("heredoc> ");
    while (getline(&line, &len, stdin) != -1) {
        if (strcmp(line, delimiter) == 0)
            break;
        char *temp = result;
        result = malloc(strlen(temp) + strlen(line) + 1);
        sprintf(result, "%s%s", temp, line);
        free(temp);
        printf("heredoc> ");
    }
    free(line);
    return result;
}

// Expansión de Variables
char *expand_variables(const char *input, char **envp) {
    char *result = strdup(input);
    for (char **env = envp; *env; env++) {
        char *key = strtok(*env, "=");
        char *value = strtok(NULL, "=");
        if (strstr(result, key)) {
            char *temp = result;
            result = malloc(strlen(temp) + strlen(value) - strlen(key) + 1);
            sprintf(result, "%s%s", strstr(temp, key) ? value : temp, strstr(temp, key + strlen(key)));
            free(temp);
        }
    }
    return result;
}

// Expansión de Comodines
char **expand_wildcards(char **cmd) {
    glob_t results;
    int flags = 0;

    glob(*cmd, flags, NULL, &results);
    char **expanded = malloc((results.gl_pathc + 1) * sizeof(char *));
    for (size_t i = 0; i < results.gl_pathc; i++)
        expanded[i] = strdup(results.gl_pathv[i]);
    expanded[results.gl_pathc] = NULL;
    globfree(&results);
    return expanded;
}

// Construcción del AST
t_ast_node *ast_build(char **argv) {
    t_ast_node *root = NULL, *current = NULL;
    while (*argv) {
        if (!strcmp(*argv, "|")) {
            t_ast_node *pipe_node = ast_create_node(NULL);
            if (current)
                current->right = pipe_node;
            else
                root = pipe_node;
            pipe_node->left = current;
            current = pipe_node;
        } else if (!strcmp(*argv, ";")) {
            if (current)
                current->right = ast_create_node(NULL);
            current = NULL;
        } else if (!strcmp(*argv, "<<")) {
            argv++;
            current->heredoc = handle_heredoc(*argv);
        } else {
            t_ast_node *cmd_node = ast_create_node(argv);
            if (!current)
                root = current = cmd_node;
            else
                current->right = cmd_node;
        }
        argv++;
    }
    return root;
}

// Ejecución del AST
int execute_ast(t_ast_node *node, char **envp) {
    if (!node)
        return 0;

    if (node->cmd) {
        if (!strcmp(node->cmd[0], "cd")) {
            if (!node->cmd[1] || node->cmd[2])
                return err("error: cd: bad arguments\n"), 1;
            if (chdir(node->cmd[1]) == -1)
                return err("error: cd: cannot change directory to "), err(node->cmd[1]), err("\n"), 1;
            return 0;
        }

        if (!strcmp(node->cmd[0], "history")) {
            print_history();
            return 0;
        }

        if (!strcmp(node->cmd[0], "cat")) {
            cat_command(node->cmd);
            return 0;
        }

        int pid = fork();
        if (pid == -1)
            return err("error: fatal\n"), 1;
        if (pid == 0) {
            if (node->heredoc) {
                int fd = open("/tmp/heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
                write(fd, node->heredoc, strlen(node->heredoc));
                close(fd);
                fd = open("/tmp/heredoc_tmp", O_RDONLY);
                dup2(fd, 0);
                close(fd);
            }
            execve(node->cmd[0], node->cmd, envp);
            err("error: cannot execute "), err(node->cmd[0]), err("\n");
            exit(1);
        }
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }

    int pipe_fd[2];
    if (node->left && pipe(pipe_fd) == -1)
        return err("error: fatal\n"), 1;

    if (node->left && fork() == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        close(pipe_fd[1]);
        exit(execute_ast(node->left, envp));
    }

    if (node->right && fork() == 0) {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], 0);
        close(pipe_fd[0]);
        exit(execute_ast(node->right, envp));
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}

// Implementación del comando cat
void cat_command(char **cmd) {
    if (!cmd[1]) {
        err("error: cat: no file provided\n");
        return;
    }

    for (int i = 1; cmd[i]; i++) {
        FILE *file = fopen(cmd[i], "r");
        if (!file) {
            err("error: cat: cannot open file ");
            err(cmd[i]);
            err("\n");
            continue;
        }

        char buffer[1024];
        size_t n;
        while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            fwrite(buffer, 1, n, stdout);
        }

        fclose(file);
    }
}

int main(int argc, char **argv, char **envp) {
    if (argc < 2)
        return err("error: no commands provided\n"), 1;

    add_to_history("Starting Shell");
    t_ast_node *ast = ast_build(argv + 1);
    int status = execute_ast(ast, envp);
    ast_free(ast);
    free_history();
    return status;
}