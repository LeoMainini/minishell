# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 17:50:39 by leferrei          #+#    #+#              #
#    Updated: 2022/11/10 18:49:23 by leferrei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

BUILTINS_DIR =	builtins


SOURCES = 	m-shell.c\
			signals.c\
			split.c\
			utils.c\
			interpreter.c\
			$(BUILTINS_DIR)/cd.c\
			$(BUILTINS_DIR)/pwd.c\
			$(BUILTINS_DIR)/env.c\
			$(BUILTINS_DIR)/exit.c\
			$(BUILTINS_DIR)/echo.c\


OBJS =	$(SOURCES:.c=.o)

CC = gcc

CFLAGS = -Wall -Werror -Wextra

DEBUG = -fsanitize=address -g

INC = -I. -I libft -I lib-pipex 

LINK = -L . -L ./libft  -L ./lib-pipex -lpipex -lft -lreadline 

.c.o: 
	$(CC) $(CFLAGS) $(INC) -c $< -o $(<:.c=.o)


$(NAME): $(OBJS)
			@make -C libft -s
			@make -C lib-pipex -s
			$(CC) $(CFLAGS) $(INC) $(DEBUG) $(OBJS) -o $(NAME) $(LINK)
all: $(NAME)

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