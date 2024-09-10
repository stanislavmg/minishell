NAME    = minishell

LIBFT    = libft.a

INCLUDE = include

PREFIX	= $(PWD)

BUILTINS =	array_utils.c builtin_utils.c cd.c cd_utils.c echo.c env.c exit.c exit_utils.c export.c\
			export_utils.c export_utils_print.c pwd.c unset.c

EXEC =		exec_utils.c exit_code.c helpers.c pipeline.c process_utils.c redirect.c signal.c travers_ast.c

LEXER = 	lex_quotes.c lex_slash.c lex_variable.c lexer_utils.c string.c token.c tokenizing.c wildcard.c

PARSING =	ast.c helpers.c here_doc.c init_cmd.c parse_cmd.c parse_process.c parser.c parser_utils.c

ENV_LST =	list_init.c list_utils.c

SRC     =	$(addprefix src/builtins/, $(BUILTINS))\
			$(addprefix src/exec/, $(EXEC))\
			$(addprefix src/lexer/, $(LEXER))\
			$(addprefix src/parsing/, $(PARSING))\
			$(addprefix src/env_list/, $(ENV_LST))\
			$(addprefix src/, main.c destructors.c)

OBJ     = $(SRC:%.c=%.o)

CFLAGS  = -g -Wall -Wextra -Werror -I$(INCLUDE) 

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) 
	$(CC) $(CFLAGS) -o $@ $^ -lreadline -L./lib

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C libft all bonus clean && mv libft/$(LIBFT) .

configure:
	cd ./readline*/ && ./configure --prefix="$(PREFIX)"
	make -C readline*/ all install

clean:
	$(RM) $(OBJ)
	$(MAKE) -C libft clean
	$(MAKE) -C readline* clean

fclean: clean
	$(RM) $(NAME) $(LIBFT)

re: fclean all

.PHONY: all configure clean fclean re 