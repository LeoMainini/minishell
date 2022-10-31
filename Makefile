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
			$(BUILTINS_DIR)/echo.c

OBJS =	$(SOURCES:.c=.o)

CC = clang

CFLAGS = -Wall -Werror -Wextra

DEBUG = #-fsanitize=address -g

INC = -I. -I libft

LINK = -L . -L ./libft -lreadline -lft

.c.o: 
	$(CC) $(CFLAGS) $(INC) -c $< -o $(<:.c=.o)

$(NAME): $(OBJS)
			@make -C libft -s
			$(CC) $(CFLAGS) $(INC) $(DEBUG) $(OBJS) -o $(NAME) $(LINK)
all: $(NAME)

re: fclean all

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME1)

.PHONY: all re clean fclean