NAME = lexer
LIB = libft.a
INCLUDE = ../../inc
SRC = lexer.c methods.c string.c parser.c main.c $(addprefix ../hashtable/, hashtable.c methods.c)
OBJ = $(SRC:%.c=%.o)
CFLAGS = -Wall -g3 -I$(INCLUDE)
CC=gcc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
lib:
	make -C ../../libft && mv ../../libft/libft.a ./
clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
