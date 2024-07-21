#ifndef PARSER_H
# define PARSER_H

# define  ERR_MEM     101
# define  ERR_TOKEN   102
# define  ERR_QUOTE   103
# define  ERR_SYNTAX  104

# include "hashtable.h"
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum t_token
{
  UNDEFINED,
  STRING,
  VARIABLE,
  COMMAND,
  //
  INPUT_TRUNC,
  OUTPUT_ADD,
  OUTPUT_TRUNC,
  AND,
  PIPE,
  OR,
  SEMICOLON,
  HERE_DOC
} e_token;

// typedef struct s_exec_cmd
// {
//     int     type;
//     int     exit_code;
//     char    **argv;
// }   t_exec_cmd;

typedef struct s_word_list
{
  char                *word;
  e_token             type;
  struct s_word_list  *next;
  struct s_word_list  *prev;
} t_word_list;

typedef struct s_lexer
{
  t_word_list *tokens;
  t_word_list *token_pos;
  t_table     *env;
  char        *input_str;
  char        *str_pos;
  int         in_qoutes;
  int         err;
} t_lexer;

typedef struct s_cmd
{
    int type;
}   t_cmd;

typedef struct s_exec_cmd
{
    int     type;
    int     file_in;
    int     file_out;
    int     exit_code;
    char    **argv;
}   t_exec_cmd;

typedef struct s_pipecmd //  | ; || &&
{
    int     type;
    t_cmd   *left;
    t_cmd   *right;
}   t_pipecmd;

// lexer methods
t_lexer	    *new_lexer(char *str);
t_word_list	*new_token(char *word, e_token type);
int	        push_token(t_word_list **list, t_word_list *new);
int	        init_word_list(t_lexer *lex);
void	      free_lexer(t_lexer *lex);

// tokenization
char	*scan_token(t_lexer *lex);
char	*metachar_handle(t_lexer *lex);
int	  default_handle(t_lexer *lex, const char *value, e_token type);
char	*string_handle(t_lexer *lex);
char	*variable_handle(t_lexer *lex);
char	*slash_handle(t_lexer *lex);
char	*single_quotes_handle(t_lexer *lex);
char	*double_quotes_handle(t_lexer *lex);

// expand tokens and build tree
int	  check_syntax(t_lexer *lex);
t_exec_cmd	*init_cmd(t_lexer *lex);
char  **split_var(char *str);
int	  insert_variable(t_lexer *lex, t_word_list *token);
t_cmd	*add_tnode(t_cmd *left_node, t_cmd *right_node, int type);
t_cmd	*create_cmd(void);
int	  add_field_fd(t_word_list *token, t_exec_cmd *cmd);
char	**add_field_argv(t_word_list *tokens, int argc);
t_cmd	*parse_tokens(t_lexer *lex);
t_cmd *build_AST(t_lexer *lex);
void  print_tree(t_pipecmd *root);

// parse utils
int	is_redirect(e_token type);
int	is_cmd_delimeter(e_token type);


// string utils
size_t	ft_strncpy(char *d, const char *s, size_t n);
char    *get_word(const char *s, size_t n);
char    *merge_str(char *s1, char *s2);
int   	is_metachar(char ch);
int	    is_catchar(char ch);

// debug
const char *get_type(int type);

#endif