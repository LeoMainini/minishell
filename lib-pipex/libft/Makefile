# This is a minimal set of ANSI/VT100 color codes
_END		=	\x1b[0m
_BOLD		=	\x1b[1m
_UNDER		=	\x1b[4m
_REV		=	\x1b[7m

# Colors
_GREY		=	\x1b[30m
_RED		=	\x1b[31m
_GREEN		=	\x1b[32m
_YELLOW		=	\x1b[33m
_BLUE		=	\x1b[34m
_PURPLE		=	\x1b[35m
_CYAN		=	\x1b[36m
_WHITE		=	\x1b[37m

# Inverted, i.e. colored backgrounds
_IGREY		=	\x1b[40m
_IRED		=	\x1b[41m
_IGREEN		=	\x1b[42m
_IYELLOW	=	\x1b[43m
_IBLUE		=	\x1b[44m
_IPURPLE	=	\x1b[45m
_ICYAN		=	\x1b[46m
_IWHITE		=	\x1b[47m

SRCS	=	ft_atoi.c		ft_bzero.c		ft_calloc.c		ft_isalnum.c	\
			ft_isalpha.c	ft_isascii.c	ft_isdigit.c	ft_isprint.c	\
			ft_itoa.c		ft_toupper.c 	ft_memchr.c		ft_memcmp.c		\
			ft_memcpy.c		ft_memmove.c	ft_memset.c		ft_split.c		\
			ft_strchr.c		ft_lstnew.c		ft_strcmp.c		get_output.c	\
			ft_strdup.c		ft_strjoin.c	ft_strlcat.c	\
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

CFLAGS	= -Wall -Werror -Wextra

.c.o:
			@${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME):	${OBJS}
			@echo -e	"$(_RED)	(      (             (              \n$(_RED)	 )\ )   )\ )     (    )\ )    *   )  \n$(_RED)	(()/(  (()/(   ( )\  (()/(    )  /(  	\n$(_RED)	 )(_))  /(_))  )((_)  /(_))  ( )(_)) \n$(_RED)	(_))   (_))   ((_)_  (_))_| (_(_())  	\n$(_PURPLE)	| |    |_ _|   | _ ) | |_   |_   _|  	\n$(_PURPLE)	| |__   | |    | _ \ | __|    | |    	\n	|____| |___|   |___/ |_|      |_|    \n"
			@echo -e	"$(_CYAN)\tCREATING AND COMPILING UPDATED OBJECT FILES\n"
			@echo -e	"$(_CYAN)\tar rc libft.a *.o\n"
			@${AR} ${NAME} ${OBJS}
			@echo -e	"$(_PURPLE)\tDONE"

all:		${NAME}
		
clean:
			@echo -e	"$(_RED)\tCLEANING FILES\n"
			@${RM} ${OBJS} ${BONUS_OBJS}
			@echo -e	"$(_CYAN)\trm -f *.c\n"
			@echo -e	"$(_PURPLE)\tDONE\n"

fclean:		clean
			@echo -e	"$(_CYAN)\trm -f libft.a\n"
			@${RM} ${NAME}
			@echo -e	"$(_PURPLE)\tDONE\n"

re:			fclean	all

bonus:		${OBJS} ${BONUS_OBJS}
			@echo -e	"$(_CYAN)\tCREATING AND COMPILING UPDATED BONUS FILES\n"
			@echo -e	"$(_CYAN)\tar rc libft.a bonus.o\n"
			@${AR} ${NAME} ${BONUS_OBJS} ${OBJS}
			@echo -e	"$(_PURPLE)\tDONE"

#so:
#	$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRCS)
#	gcc -nostartfiles -shared -o libft.so $(OBJS)

.PHONY:		all clean fclean re
