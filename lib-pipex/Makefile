# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/10 18:30:02 by leferrei          #+#    #+#              #
#    Updated: 2022/11/14 17:32:58 by bcarreir         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRCS	=	pipex.c\
			utils.c\
			process_utils.c\
			init_utils.c\
			argument_processing.c\
			path_finding.c\
			get_next_line/get_next_line.c\
			get_next_line/get_next_line_utils.c\
			stdin_parsing.c

TEST_SRCS	=	test.c

OBJS	=	${SRCS:.c=.o}

TEST_OBJS	=	$(OBJS) ${TEST_SRCS:.c=.o}

LFT_NAME = ../libft/libft.a

NAME	=	libpipex.a

ifeq ($(shell uname),Linux)
	ECHO	=	echo -e
else
	ECHO	=	echo
endif

CC		=	cc

RM		=	rm -f

AR		=	ar rc

CFLAGS	=	-Wall -Werror -Wextra -D BUFFER_SIZE=1 -g# -fsanitize=address -g

LIBFLAGS =  -fPIE

INC		=	 -I ../libft -L../libft -lft -Lget_next_line -I . -I .. 

ifeq ($($@), test)
	OBJ_COMPILE = @$(CC) $(CFLAGS) -fPIC -c $< -o $@
else
	OBJ_COMPILE = @$(CC) $(CFLAGS) $(LIBFLAGS) -c $< -o $@
endif

%.o: %.c
	$(OBJ_COMPILE)

$(NAME):	$(OBJS)
			@$(AR) $(NAME) $(OBJS)

$(LFT_NAME):
			@make -C ../libft

all:		${NAME}

clean:	
			
			@${RM} ${OBJS}
			@make -C ../libft clean

fclean:		clean
			@${RM} ${NAME}
			@make -C ../libft fclean

test:		fclean $(LFT_NAME) $(TEST_OBJS) 
			$(CC) $(CFLAGS) $(INC) $(TEST_OBJS) -o pipex

re:			fclean	all

