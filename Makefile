NAME = minishell

BUILTINS_DIR =	builtins

SOURCES = 	m-shell.c signals.c $(BUILTINS_DIR)/cd.c\

OBJS =	$(SOURCES:.c=.o)

CC = gcc

CFLAGS = -Wall -Werror -Wextra

DEBUG = -fsanitize=address -g

INC = -I. -I libft

LINK = -L . -L ./libft -lreadline -lft

.c.o: $(CC) $(CFLAGS) $(INC) -c $< -o $(<:.c=.o)

$(NAME): $(OBJS) $(NAME)
			$(CC) $(CFLAGS) $(OBJS) $(LINK) -o $(NAME)

all: $(NAME)

re: fclean all

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME1)

.PHONY: all re clean fclean