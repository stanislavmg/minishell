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
