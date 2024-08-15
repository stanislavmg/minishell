NAME    = minishell
LIB     = libft.a
INCLUDE = ../../include
MAIN 	?= main.c

SRC     = parser.c init_cmd.c debug.c destructors.c parser_utils.c here_doc.c ast.c wildcard.c\
		  $(wildcard lexer/*) $(wildcard ../env_list/*) $(wildcard ../exec/*)\
		  $(wildcard ../ft_printf/*) $(wildcard ../builtins/*)

LEX_SRC = debug.c\
		  $(addprefix lexer/, lexer.c lexer_main.c methods.c string.c lexer_utils.c) \
          $(addprefix ../env_list/, list_init.c list_utils.c)\

LEX_OBJ = $(LEX_SRC:%.c=%.o)

OBJ     = $(SRC:%.c=%.o)

CFLAGS  = -Wall -g -I$(INCLUDE) -fsanitize=address 

all: $(NAME)

$(NAME): $(OBJ) $(MAIN:.c=.o) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

%.o: %.c $(MAIN)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	 make -C ../../libft all bonus clean && mv ../../libft/$(LIB) .

lexer: $(LEX_OBJ)
	$(CC) $(CFLAGS) -o lex $(LEX_OBJ) $(LIB)

clean:
	$(RM) $(OBJ) $(LEX_OBJ) $(MAIN:.c=.o)

fclean: clean
	$(RM) $(NAME) lex

re: fclean all

.PHONY: all clean fclean re lib lexer
