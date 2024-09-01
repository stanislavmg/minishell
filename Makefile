NAME    = minishell

LIB     = libft.a

INCLUDE = include

BUILTINS =	array_utils.c builtin_utils.c cd.c cd_utils.c echo.c env.c exit.c exit_utils.c export.c\
			export_utils.c export_utils_print.c pwd.c unset.c
EXEC =		exec_utils.c exit_code.c helpers.c pipeline.c process_utils.c redirect.c signal.c travers_ast.c

LEXER = 	lex_quotes.c lex_slash.c lex_variable.c lexer_utils.c string.c token.c tokenizing.c wildcard.c

PARSING =	ast.c helpers.c here_doc.c init_cmd.c parse_cmd.c parse_process.c parser.c parser_utils.c

ENV_LST =	list_init.c list_utils.c

SRC     =	$(addprefix src/parsing/, $(PARSING))\
			$(addprefix src/env_list/, $(ENV_LST))\
			$(addprefix src/lexer/, $(LEXER))  $(addprefix src/exec/, $(EXEC))\
			$(addprefix src/builtins/, $(BUILTINS)) $(addprefix src/, main.c destructors.c debug.c)

OBJ     = $(SRC:%.c=%.o)

CFLAGS  = -g -Wall -I$(INCLUDE)

DFLAGS	= -fsanitize=undefined -fsanitize=address

READLINE := ./readline_config.sh

all: $(NAME)

$(NAME): $(OBJ) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	make -C libft all bonus clean && mv libft/$(LIB) .

clean:
	$(RM) $(OBJ)

configure:
	bash $(READLINE)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: CFLAGS += $(DFLAGS)

debug: fclean all

.PHONY: all clean fclean re configure debug
