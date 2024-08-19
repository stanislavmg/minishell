NAME    = minishell
LIB     = libft.a
INCLUDE = include
MAIN 	?= src/parsing/main.c
READLINE = 
SRC     = $(wildcard src/parsing/*) $(wildcard src/env_list/*)\
		  $(wildcard src/lexer/*)  $(wildcard src/exec/*)\
		  $(wildcard src/builtins/*)

OBJ     = $(SRC:%.c=%.o)

CFLAGS  = -Wall -g -I$(INCLUDE) -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ) $(MAIN:.c=.o) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

%.o: %.c $(MAIN)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	 make -C libft all bonus clean && mv libft/$(LIB) .

lexer: $(LEX_OBJ)
	$(CC) $(CFLAGS) -o lex $(LEX_OBJ) $(LIB)

clean:
	$(RM) $(OBJ) $(LEX_OBJ) $(MAIN:.c=.o)

fclean: clean
	$(RM) $(NAME) lex

re: fclean all

.PHONY: all clean fclean re lib lexer
