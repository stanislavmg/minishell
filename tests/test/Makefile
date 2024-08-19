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
