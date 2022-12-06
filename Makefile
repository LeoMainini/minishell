# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 17:50:39 by leferrei          #+#    #+#              #
#    Updated: 2022/12/06 15:47:30 by leferrei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

BUILTINS_DIR =	builtins


SOURCES = 	$(BUILTINS_DIR)/cd.c\
			$(BUILTINS_DIR)/cd_utils.c\
			$(BUILTINS_DIR)/echo.c\
			$(BUILTINS_DIR)/env.c\
			$(BUILTINS_DIR)/env_utils.c\
			$(BUILTINS_DIR)/exit.c\
			$(BUILTINS_DIR)/export.c\
			$(BUILTINS_DIR)/pwd.c\
			$(BUILTINS_DIR)/unset.c\
			builtin_exec_utils.c\
			cmd_ac_size.c\
			cmd_redirs.c\
			cmd_redirs2.c\
			cmd_validation.c\
			cmd_validation2.c\
			env_var.c\
			execution.c\
			file_utils.c\
			getters_setters.c\
			here_doc_utils.c\
			interpreter.c\
			interpreter_utils.c\
			m-shell_routine_utils.c\
			mem_utils.c\
			shell_path_utils.c\
			signals.c\
			split.c\
			stdin_parsing.c\
			system_exec_utils.c\
			utils.c\
			m-shell.c\


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