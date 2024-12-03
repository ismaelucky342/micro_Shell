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
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <glob.h>

// Nodo del árbol de sintaxis abstracta (AST)
typedef struct s_ast_node {
    char **cmd;                    // Comando y sus argumentos
    char *heredoc;                 // Heredoc si existe
    struct s_ast_node *left;       // Nodo izquierdo (para pipes)
    struct s_ast_node *right;      // Nodo derecho (para más comandos)
} t_ast_node;

// Historial de comandos
typedef struct s_history {
    char *command;
    struct s_history *next;
} t_history;

t_history *g_history = NULL;

#endif // MICRO_SHELL_H