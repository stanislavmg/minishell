NAME = test
LIB = libft.a
INCLUDE = ../../inc
SRC = lexer.c methods.c string.c main.c $(addprefix ../hashtable/, hashtable.c methods.c)
OBJ = $(SRC:%.c=%.o)
CFLAGS = -Wall -g3 -I$(INCLUDE)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(LIB):
	make -C ../../libft && mv ../../libft/libft.a ./

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
