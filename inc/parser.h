#ifndef PARSER_H
# define PARSER_H

# include "hashtable.h"
# include <stdlib.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>


int cd(char **ar, t_table *env);

typedef enum t_token
{
  UNDEFINED,
  STRING,
  VARIABLE,
  COMMAND,
  //
  INPUT_ADD,
  INPUT_TRUNC,
  OUTPUT_ADD,
  OUTPUT_TRUNC,
  ASSIGNMENT,
  AND,
  PIPE,
  OR,
  SEMICOLON,
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
  t_table     *env;
  char        *pos;
} t_lexer;

// abstract syntax tree
typedef struct s_ast 
{
  struct s_ast  *left;
  struct s_ast  *right;
  //char          *value;
  e_token       type;
} t_ast;

// lexer methods
t_lexer	    *new_lexer(char *str);
t_word_list	*new_token(char *word, e_token type);
int	        push_token(t_word_list **list, t_word_list *new);
int	        init_word_list(t_lexer *lex);

// tokenization
char	*scan_token(t_lexer *lex);
int	  metachar_handle(t_lexer *lex);
int	  default_handle(t_lexer *lex, const char *value, e_token type);
char	*string_handle(t_lexer *lex);
char	*variable_handle(t_lexer *lex);
char	*slash_handle(t_lexer *lex);
char	*single_quotes_handle(t_lexer *lex);
char	*double_quotes_handle(t_lexer *lex);


// string utils
size_t	ft_strncpy(char *d, const char *s, size_t n);
char    *get_word(const char *s, size_t n);
char    *merge_str(char *s1, char *s2);
int   	is_metachar(char ch);
int	    is_catchar(char ch);
#endif