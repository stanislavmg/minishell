NAME    = minishell
LIB     = libft.a
INCLUDE = include
SRC     = $(wildcard src/parsing/*) $(wildcard src/env_list/*)\
		  $(wildcard src/lexer/*)  $(wildcard src/exec/*)\
		  $(wildcard src/builtins/*)

OBJ     = $(SRC:%.c=%.o)
OS := $(shell uname)
CFLAGS  = -Wall -I$(INCLUDE)

ifeq ($(OS), Darwin)
    READLINE := ./readline_config.sh
endif

ifeq ($(OS), Darwin)
    all: $(READLINE) $(NAME)
else
    all: $(NAME)
endif

$(NAME): $(OBJ) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB):
	make -C libft all bonus clean && mv libft/$(LIB) .

$(READLINE):
	bash $(READLINE)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
