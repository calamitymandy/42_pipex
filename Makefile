# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/23 17:26:38 by amdemuyn          #+#    #+#              #
#    Updated: 2023/11/23 17:26:42 by amdemuyn         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

YELLOW =		\033[0;93m
RED =			\033[0;91m
GREEN =			\033[0;92m

NAME = pipex
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 
#-fsanitize=address  -O

SRC = main.c utils.c split.c

OBJ = ${SRC:.c=.o}


all: ${NAME}

${NAME}: ${OBJ}
		${CC} ${CFLAGS} ${OBJ} -o ${NAME}
		@echo "\n$(GREEN)✅ Objects and executables created successfully!\n"

clean:
	rm -rf ${OBJ}
	@echo "\n$(YELLOW)✅ Objects cleaned successfully!\n"

fclean: clean
	rm -rf ${NAME}
	rm -rf ${OBJ}
	@echo "\n$(RED)✅ Objects and executables deleted successfully!\n"

re: fclean all

.PHONY: all clean fclean re