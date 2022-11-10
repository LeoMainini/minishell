# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/10 18:30:02 by leferrei          #+#    #+#              #
#    Updated: 2022/11/10 18:49:51 by leferrei         ###   ########.fr        #
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


OBJS	=	${SRCS:.c=.o}



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

CFLAGS	=	-Wall -Werror -Wextra -D BUFFER_SIZE=1 -g -fPIE#-fsanitize=address -g

INC		=	 -L../libft -Lget_next_line -I . -I .. -lft

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJS)
			@$(AR) $(NAME) $(OBJS)


$(LFT_NAME):
			@make -C ../libft

all:		${NAME}

clean:	
			
			@$(ECHO) "\033[31mCleaning files in fancy colors"
			@${RM} ${OBJS}
			@make -C ../libft clean

fclean:		clean
			@${RM} ${NAME}
			@make -C ../libft fclean

re:			fclean	all

