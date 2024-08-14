<<<<<<< HEAD:src/builtins/Makefile
NAME = builtins
SRC = $(wildcard *.c) $(addprefix ../env_list/, list_init.c list_utils.c)
OBJ = $(SRC:.c=.o)
LIB = libft.a
CFLAGS = -Wall -g3 -fsanitize=address -I../../include

all : $(NAME)

$(NAME) : $(OBJ) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^
$(LIB) :
	make -C ../../libft all bonus clean && mv ../../libft/$(LIB) .

clean :
	$(RM) $(OBJ)
=======
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/12 14:40:01 by sgoremyk          #+#    #+#              #
#    Updated: 2024/08/12 15:21:20 by sgoremyk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    =   libftprintf.a

INCLUDE =   -I.

CFLAGS  =   -Wall -Wextra -Werror $(INCLUDE)

SRC     =   ft_printf.c internal_ft_printf.c ft_out.c

OBJ     =   $(SRC:%.c=%.o)

.PHONY  :   all clean fclean re

all     :   $(NAME)

$(NAME) : $(OBJ)
	ar rcs $@ $^

%.o    : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean  :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re     : fclean all
>>>>>>> origin/parser:src/ft_printf/Makefile
