# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 17:50:39 by leferrei          #+#    #+#              #
#    Updated: 2023/01/24 17:31:34 by leferrei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#COLORS	||	0; -> Reg	|	1; -> Bold	|	4; -> Underline	|	;9 -> H. Intensity	||
Color_Off	= \033[0m
Black		= \033[0;30m
Red			= \033[0;31m
Green		= \033[0;32m
Yellow		= \033[0;33m
Blue		= \033[0;34m
Purple		= \033[0;35m
Cyan		= \033[0;36m
White		= \033[0;37m
BBlack		= \033[1;30m
BRed		= \033[1;31m
BGreen		= \033[1;32m
BYellow		= \033[1;33m
BBlue		= \033[1;34m
BPurple		= \033[1;35m
BCyan		= \033[1;36m
BWhite		= \033[1;37m
UBlack		= \033[4;30m
URed		= \033[4;31m
UGreen		= \033[4;32m
UYellow		= \033[4;33m
UBlue		= \033[4;34m
UPurple		= \033[4;35m
UCyan		= \033[4;36m
UWhite		= \033[4;37m
IBlack		= \033[0;90m
IRed		= \033[0;91m
IGreen		= \033[0;92m
IYellow		= \033[0;93m
IBlue		= \033[0;94m
IPurple		= \033[0;95m
ICyan		= \033[0;96m
IWhite		= \033[0;97m

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
			cmd_redirs2.c\
			cmd_redirs.c\
			cmd_validation2.c\
			cmd_validation.c\
			data_checks.c\
			env_var.c\
			execution.c\
			file_utils.c\
			getters_setters.c\
			here_doc_utils.c\
			interpreter.c\
			interpreter_utils.c\
			m-shell.c\
			m-shell_routine_inter_utils.c\
			m-shell_routine_utils.c\
			mem_utils.c\
			shell_path_utils.c\
			signals.c\
			split.c\
			split_frees.c\
			stdin_parsing.c\
			system_exec_utils.c\
			utils.c

SRCS = $(addprefix $(SRC_DIR), $(SOURCES))

OBJECTS = $(SOURCES:.c=.o)

OBJS =	$(addprefix $(OBJ_DIR), $(OBJECTS))

CC = gcc

LFT = $(LIBS_FOLDER)libft/libft.a

CFLAGS = -Wall -Werror -Wextra

DEBUG = -g -fsanitize=address

INC = -I. -I $(LIBS_FOLDER)libft

LINK = -L . -L $(LIBS_FOLDER)/libft -lft -lreadline 

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@printf "$(Purple)Compiling $@\n$(Color_Off)"
	@$(CC) $(CFLAGS) $(DEBUG) $(INC) -c $< -o $@

all:		$(NAME)

$(NAME):	$(LFT)	$(OBJS)  $(SRCS)
			@printf "$(IPurple)Compiling object files into $(NAME)\n$(Color_Off)"
			@$(CC) $(CFLAGS) $(INC) $(DEBUG) $(OBJS) -o $(NAME) $(LINK)
			@clear
			@printf "\n\t$(BBlue)Compilation complete\n $(Color_Off)\n"

$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)$(BUILTINS_DIR)

$(LFT):		$(LIBS_FOLDER)
			@make -C $(LIBS_FOLDER)libft -s			

re: fclean all

clean:
	@make -C $(LIBS_FOLDER)libft clean -s
	@printf "$(IRed)Removing object files\n$(Color_Off)"
	@rm -f $(OBJS)

fclean: clean
	@make -C $(LIBS_FOLDER)libft fclean -s
	@printf "$(Red)Removing program files\n$(Color_Off)"
	@rm -f $(NAME)

.PHONY: all re clean fclean
