NAME    = minishell
LIB     = libft.a
PRINTF	= libftprintf.a
INCLUDE = ../../inc
MAIN 	?= main.c

SRC     = parser.c init_cmd.c debug.c destructors.c parser_utils.c here_doc.c ast.c\
		  $(addprefix lexer/, lexer.c methods.c string.c lexer_utils.c)\
          $(addprefix ../env_list/, list_init.c list_utils.c)\
          $(addprefix ../exec/, exec.c exec_utils.c)

LEX_SRC = $(addprefix lexer/, lexer.c lexer_main.c methods.c string.c lexer_utils.c) \
          $(addprefix ../env_list/, list_init.c list_utils.c)\
		  $(addprefix ../, debug.c)
		  

LEX_OBJ = $(LEX_SRC:%.c=%.o)

OBJ     = $(SRC:%.c=%.o)

CFLAGS  = -Wall -g3 -I$(INCLUDE) #-fsanitize=address 

all: $(NAME)

$(NAME): $(OBJ) $(MAIN:.c=.o) $(LIB) $(PRINTF)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PRINTF):
	make -C ../ft_printf all clean && mv ../ft_printf/$(PRINTF) .

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
