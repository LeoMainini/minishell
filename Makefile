# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 17:50:39 by leferrei          #+#    #+#              #
#    Updated: 2022/12/06 15:08:22 by leferrei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

BUILTINS_DIR =	builtins


SOURCES = 	m-shell.c\
			cmd_ac_size.c\
			cmd_redirs.c\
			cmd_redirs2.c\
			cmd_validation.c\
			signals.c\
			split.c\
			utils.c\
			interpreter.c\
			interpreter_utils.c\
			env_var.c\
			builtin_exec_utils.c\
			system_exec_utils.c\
			file_utils.c\
			getters_setters.c\
			mem_utils.c\
			here_doc_utils.c\
			execution.c\
			m-shell_routine_utils.c\
			stdin_parsing.c\
			shell_path_utils.c\
			$(BUILTINS_DIR)/cd.c\
			$(BUILTINS_DIR)/pwd.c\
			$(BUILTINS_DIR)/env.c\
			$(BUILTINS_DIR)/exit.c\
			$(BUILTINS_DIR)/echo.c\
			$(BUILTINS_DIR)/export.c\
			$(BUILTINS_DIR)/unset.c\
			$(BUILTINS_DIR)/env_utils.c\
			$(BUILTINS_DIR)/cd_utils.c


OBJS =	$(SOURCES:.c=.o)

CC = gcc

LFT = libft/libft.a

CFLAGS = -Wall -Werror -Wextra

DEBUG = -fsanitize=address -g

INC = -I. -I libft

LINK = -L . -L ./libft -lft -lreadline 

%.o: %.c
	@printf "Compiling $@\n"
	@$(CC) $(CFLAGS) $(DEBUG) $(INC) -c $< -o $@

$(NAME):	$(LFT)	$(OBJS)
			@printf "Compiling object files into $(NAME)\n"
			@$(CC) $(CFLAGS) $(INC) $(DEBUG) $(OBJS) -o $(NAME) $(LINK)

$(LFT):
			@make -C libft -s

all:		$(NAME)
			

re: fclean all

clean:
	@make -C libft clean -s
	@printf "Removing object files\n"
	@rm -f $(OBJS)

fclean: clean
	@make -C libft fclean -s
	@printf "Removing program files\n"
	@rm -f $(NAME)

.PHONY: all re clean fclean