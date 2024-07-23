NAME = pipex

LIB = libft/libft.a

INCLUDE = pipex.h

SRCS = main.c initialization.c utils.c

OBJ = $(SRCS:%.c=%.o)

CFLAGS = -Wall -Wextra -Werror

all		:	$(NAME)

$(NAME)	:	$(OBJ) $(INCLUDE) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

$(LIB)	:
	$(MAKE) -C libft

%.o		:	%.c $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean	:
	$(MAKE) -C libft clean
	$(RM) $(OBJ)

fclean	:	clean
	$(MAKE) -C libft fclean
	$(RM) $(NAME)

re		:	fclean all

.PHONY	:	all clean fclean re
