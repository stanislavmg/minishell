# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/01 17:05:52 by sgoremyk          #+#    #+#              #
#    Updated: 2024/09/10 21:05:31 by sgoremyk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a

SRCS	=	ft_isalpha.c	ft_strlcpy.c	ft_strtrim.c\
			ft_isdigit.c	ft_strlen.c		ft_split.c\
			ft_isalnum.c	ft_strlcat.c	ft_strmapi.c\
			ft_isascii.c	ft_strncmp.c	ft_striteri.c\
			ft_isprint.c	ft_strchr.c		ft_putchar_fd.c\
			ft_toupper.c	ft_strrchr.c	ft_putstr_fd.c\
			ft_tolower.c	ft_memchr.c		ft_putendl_fd.c\
			ft_atoi.c		ft_memcmp.c		ft_putnbr_fd.c\
			ft_itoa.c		ft_calloc.c		ft_substr.c\
			ft_strdup.c		ft_bzero.c		ft_strjoin.c\
			ft_memcpy.c		ft_memset.c		ft_memmove.c\
			ft_strnstr.c	ft_open.c		ft_close.c\
			ft_isspace.c	ft_strcmp.c

SRCS_B	=	ft_lstclear.c	ft_lstnew.c		ft_lstadd_front.c\
			ft_lstsize.c	ft_lstiter.c	ft_lstadd_back.c\
			ft_lstlast.c	ft_lstmap.c		ft_lstdelone.c

HEADER	=	../include/libft.h
OBJ		=	$(SRCS:%.c=%.o)
OBJ_B	=	$(SRCS_B:%.c=%.o)

CC		=	clang
CFLAGS	=	-Wall -Wextra -Werror -I../include

.PHONY	:	all clean fclean re bonus

all		:	$(NAME)

$(NAME)	:	$(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o		:	%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

bonus	:	$(OBJ) $(OBJ_B)
	ar rcs $(NAME) $(OBJ) $(OBJ_B)

clean	:
	rm -f $(OBJ) $(OBJ_B)

fclean	:	clean
	rm -f $(NAME)

re		:	fclean all
