SRCS	=	ft_atoi.c		ft_bzero.c		ft_calloc.c		ft_isalnum.c	\
			ft_isalpha.c	ft_isascii.c	ft_isdigit.c	ft_isprint.c	\
			ft_itoa.c		ft_toupper.c 	ft_memchr.c		ft_memcmp.c		\
			ft_memcpy.c		ft_memmove.c	ft_memset.c		ft_split.c		\
			ft_strchr.c		ft_lstnew.c		ft_strcmp.c		get_output.c	\
			ft_strdup.c		ft_strjoin.c	ft_strlcat.c	ft_isspace.c	\
			ft_strlen.c		ft_tolower.c	ft_strncmp.c	\
			ft_strnstr.c	ft_strrchr.c	ft_substr.c		\
			ft_strlcpy.c	ft_strtrim.c	ft_striteri.c	ft_strmapi.c	\
			ft_putchar_fd.c	ft_putstr_fd.c	ft_putendl_fd.c	ft_putnbr_fd.c	\
			ft_strstr.c

BONUS	=	ft_lstadd_front.c				ft_lstsize.c	ft_lstlast.c	\
			ft_lstadd_back.c				ft_lstdelone.c	ft_lstclear.c	\
			ft_lstiter.c	ft_lstmap.c

OBJS	= ${SRCS:.c=.o}

BONUS_OBJS = ${BONUS:.c=.o}

NAME	= libft.a

CC		= cc

RM		= rm -f

AR		= ar rc

CFLAGS	= -Wall -Werror -Wextra -fPIE

.c.o:
			@${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME):	${OBJS}
			@${AR} ${NAME} ${OBJS}

all:		${NAME}
		
clean:
			@${RM} ${OBJS} ${BONUS_OBJS}

fclean:		clean
			@${RM} ${NAME}

re:			fclean	all

bonus:		${OBJS} ${BONUS_OBJS}
			@${AR} ${NAME} ${BONUS_OBJS} ${OBJS}

#so:
#	$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRCS)
#	gcc -nostartfiles -shared -o libft.so $(OBJS)

.PHONY:		all clean fclean re
