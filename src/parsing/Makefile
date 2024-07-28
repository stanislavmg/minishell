NAME    = parser
LIB     = libft.a
INCLUDE = ../../inc

SRC     = parser.c main.c init_cmd.c syntax_check.c print_tree.c destruction.c\
		  $(addprefix lexer/, lexer.c methods.c string.c utils.c)\
          $(addprefix ../env_list/, list_init.c list_utils.c)

LEX_SRC = $(addprefix lexer/, lexer.c lexer_main.c methods.c string.c utils.c) \
          $(addprefix ../env_list/, list_init.c list_utils.c)

LEX_OBJ = $(LEX_SRC:%.c=%.o)
OBJ     = $(SRC:%.c=%.o)

CFLAGS  = -Wall -g3 -fsanitize=address -I$(INCLUDE)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

lib:
	 make -C ../../libft all bonus clean && mv ../../libft/$(LIB) ./

lexer: $(LEX_OBJ)
	$(CC) $(CFLAGS) -o lex $(LEX_OBJ) $(LIB)

clean:
	$(RM) $(OBJ) $(LEX_OBJ)

fclean: clean
	$(RM) $(NAME) lex

re: fclean all

.PHONY: all clean fclean re lib lexer
