SRCS	=	ft_putbase10_plus.c	ft_putnbr_fd.c	ft_strlen.c	\
			ft_putchar_fd.c		ft_putstr_fd.c	ft_printf.c	\
			ft_printmemory.c

OBJS	= ${SRCS:.c=.o}

NAME	= libftprintf.a

CC		= cc

RM		= rm -f

AR		= ar rc

CFLAGS	= -Wall -Werror -Wextra

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME):	${OBJS}
			${AR} ${NAME} ${OBJS}

all:		${NAME}

clean:			
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean	all

