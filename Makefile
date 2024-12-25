# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ismherna <ismherna@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/03 12:17:42 by ismherna          #+#    #+#              #
#    Updated: 2024/12/03 12:29:15 by ismherna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nombre del ejecutable
NAME = microshell

# Compilador y opciones
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iincludes
LDFLAGS = -lreadline

# Directorios
SRCDIR = src
INCDIR = includes

# Archivos fuente y objetos
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Colores
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m # No Color

# Regla por defecto
all: $(NAME)

# Compilación del ejecutable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)
	@echo  "$(GREEN)Compilación completa!$(NC)"	
	@echo  "$(GREEN)		███╗   ███╗██╗ ██████╗██████╗  ██████╗ ███████╗██╗  ██╗███████╗██╗     ██╗$(NC)"
	@echo  "$(GREEN)		████╗ ████║██║██╔════╝██╔══██╗██╔═══██╗██╔════╝██║  ██║██╔════╝██║     ██║$(NC)"
	@echo  "$(GREEN)		██╔████╔██║██║██║     ██████╔╝██║   ██║███████╗███████║█████╗  ██║     ██║$(NC)"
	@echo  "$(GREEN)		██║╚██╔╝██║██║██║     ██╔══██╗██║   ██║╚════██║██╔══██║██╔══╝  ██║     ██║$(NC)"
	@echo  "$(GREEN)		██║ ╚═╝ ██║██║╚██████╗██║  ██║╚██████╔╝███████║██║  ██║███████╗███████╗███████╗$(NC)"
	@echo  "$(GREEN)		╚═╝     ╚═╝╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝$(NC)"
	@echo  "$(GREEN)		      03/12/2024 - ismherna@student.42.fr - 42 Madrid & U-Tad- Ismael Hernández$(NC)"		
# Regla para crear los archivos objeto
%.o: %.c $(INCDIR)/microshell.h
	@echo -e "$(YELLOW)Compilando $<...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza de archivos compilados
clean:
	@echo -e "$(RED)Limpiando archivos objeto...$(NC)"
	rm -f $(OBJS)

# Limpieza total
fclean: clean
	@echo -e "$(RED)Limpiando todo...$(NC)"
	rm -f $(NAME)

# Reconstrucción completa
re: fclean all

# Declaración de phony
.PHONY: all clean fclean re
