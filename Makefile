# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 17:50:39 by leferrei          #+#    #+#              #
#    Updated: 2022/12/05 14:48:10 by leferrei         ###   ########.fr        #
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

PP = lib-pipex/libpipex.a

LFT = libft/libft.a

CFLAGS = -Wall -Werror -Wextra

DEBUG = -fsanitize=address -g

INC = -I. -I libft -I lib-pipex 

LINK = -L . -L ./libft  -L ./lib-pipex -lpipex -lft -lreadline 

.c.o: 
	$(CC) $(CFLAGS) $(DEBUG) $(INC) -c $< -o $(<:.c=.o)

$(NAME):	$(LFT)	$(PP)	$(OBJS)
			$(CC) $(CFLAGS) $(INC) $(DEBUG) $(OBJS) -o $(NAME) $(LINK)

$(PP):
			@make -C lib-pipex -s

$(LFT):
			@make -C libft -s

all:		$(NAME)
			

re: fclean all

clean:
	@make -C libft clean -s
	@make -C lib-pipex clean -s 
	rm -f $(OBJS)

fclean: clean
	@make -C libft fclean -s
	@make -C lib-pipex fclean -s
	rm -f $(NAME)

.PHONY: all re clean fclean