# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 17:50:39 by leferrei          #+#    #+#              #
#    Updated: 2023/01/18 16:15:34 by leferrei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

BUILTINS_DIR =	builtins/

SRC_DIR = src/

OBJ_DIR = obj/

LIBS_FOLDER = lib/

SOURCES = 	$(BUILTINS_DIR)cd.c\
			$(BUILTINS_DIR)cd_utils.c\
			$(BUILTINS_DIR)echo.c\
			$(BUILTINS_DIR)env.c\
			$(BUILTINS_DIR)env_utils.c\
			$(BUILTINS_DIR)exit.c\
			$(BUILTINS_DIR)export.c\
			$(BUILTINS_DIR)pwd.c\
			$(BUILTINS_DIR)unset.c\
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
			m-shell_routine_inter_utils.c\
			mem_utils.c\
			shell_path_utils.c\
			signals.c\
			split.c\
			stdin_parsing.c\
			system_exec_utils.c\
			utils.c\
			m-shell.c

SRCS = $(addprefix $(SRC_DIR), $(SOURCES))

OBJECTS = $(SOURCES:.c=.o)

OBJS =	$(addprefix $(OBJ_DIR), $(OBJECTS))

CC = gcc

LFT = $(LIBS_FOLDER)libft/libft.a

CFLAGS = -Wall -Werror -Wextra

DEBUG = #-g -fsanitize=address

INC = -I. -I $(LIBS_FOLDER)libft

LINK = -L . -L $(LIBS_FOLDER)/libft -lft -lreadline 

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@printf "Compiling $@\n"
	@$(CC) $(CFLAGS) $(DEBUG) $(INC) -c $< -o $@

all:		$(NAME)

$(NAME):	$(LFT)	$(OBJS)  $(SRCS)
			@printf "Compiling object files into $(NAME)\n"
			@$(CC) $(CFLAGS) $(INC) $(DEBUG) $(OBJS) -o $(NAME) $(LINK)

$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)$(BUILTINS_DIR)

$(LFT):		$(LIBS_FOLDER)
			@make -C $(LIBS_FOLDER)libft -s			

re: fclean all

clean:
	@make -C $(LIBS_FOLDER)libft clean -s
	@printf "Removing object files\n"
	@rm -f $(OBJS)

fclean: clean
	@make -C $(LIBS_FOLDER)libft fclean -s
	@printf "Removing program files\n"
	@rm -f $(NAME)

.PHONY: all re clean fclean