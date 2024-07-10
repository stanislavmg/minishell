#ifndef PARSER_H
# define PARSER_H
#include <stdlib.h>
# include "../../inc/minishell.h"

typedef enum t_token
{
  UNDEFINED,
  STRING,
  CHARACTER,
  STR_ONE_Q,
  STR_DOUBLE_Q,
  VARIABLE,
  COMMAND,
// metachar
  INPUT_ADD,
  INPUT_TRUNC,
  OUTPUT_ADD,
  OUTPUT_TRUNC,
  ASSIGNMENT,
  AND,
  OR,
  SEMICOLON,
  PIPE,
  TILDA
} e_token;

typedef struct s_word_list
{
  char                *word;
  e_token             type;
  struct s_word_list  *next;
} t_word_list;

typedef struct s_lexer
{
  t_word_list *tokens;
  char        *str;
  char        *pos;
} t_lexer;

// abstract syntax tree
typedef struct s_ast
{
  struct s_ast  *left;
  struct s_ast  *right;
  char          *value;
  e_token       type;
} t_ast;

// lexer methods
int	    init_tokens(t_lexer *lex);
t_word_list	*new_token(const char *word, e_token type);
int	push_token(t_word_list **list, t_word_list *new);
t_lexer	*new_lexer(char *str);
// token
int	single_quotes_handle(t_lexer *lex);
int	string_handle(t_lexer *lex);
int	default_handle(t_lexer *lex, const char *value, e_token type);
int	init_tokens(t_lexer *lex);

#endif