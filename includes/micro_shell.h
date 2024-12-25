/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_shell.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ismherna <ismherna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:17:46 by ismherna          #+#    #+#             */
/*   Updated: 2024/12/03 12:17:47 by ismherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICRO_SHELL_H
#define MICRO_SHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

// Definición de colores para la salida en la terminal
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

// Historial de comandos
typedef struct s_history {
    char *command;
    struct s_history *next;
} t_history;

// Prototipos
void err(const char *msg);
void add_to_history(t_history **history, const char *cmd);
void free_history(t_history *history);
void execute_command(char **cmd, char **envp);
void shell_loop(char **envp);

#endif // MICRO_SHELL_H
