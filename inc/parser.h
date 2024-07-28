#ifndef PARSER_H
# define PARSER_H

# include "env.h"
# include "error.h"
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
  ASSIGNMENT,
  INPUT_TRUNC,
  OUTPUT_ADD,
  OUTPUT_TRUNC,
  AND,
  PIPE,
  OR,
  SEMICOLON,
  HERE_DOC,
  OPEN_BRACKET,
  CLOSED_BRACKET
} e_token;

typedef struct s_cmd
{
    e_token type;
}   t_cmd;

typedef struct s_ast
{
    e_token type;
    t_cmd   *left;
    t_cmd   *right;
}   t_ast;

typedef struct s_var
{
    e_token type;
    char    *key;
    char    *value;
}   t_var;

typedef struct s_exec_cmd
{
    e_token     type;
    char        *path;
    char        **argv;
    char        *in_fname;
    char        *out_fname;
    int         in_fmode;
    int         out_fmode;
    int         exit_code;
}   t_exec_cmd;

typedef struct s_word_list
{
  char                *word;
  e_token             type;
  struct s_word_list  *next;
  struct s_word_list  *prev;
} t_word_list;

typedef struct s_parser
{
  t_word_list *token;
  t_ast       *cmd_tree;
  char        **paths;
  t_env       *env_lst;
  int         cmd_start;
  int         brackets_count;
  int         err;
} t_parser;

typedef struct s_lexer
{
  t_word_list *tokens;
  t_word_list *token_pos;
  t_env       *env;
  char        *input_str;
  char        *str_pos;
  int         in_qoutes;
  int         err;
} t_lexer;

// lexer methods
t_lexer	    *new_lexer(char *input, t_env *env_list);
t_word_list	*new_token(char *word, e_token type);
int	        push_token(t_word_list **list, t_word_list *new);
int	        init_word_list(t_lexer *lex);
void	      free_lexer(t_lexer *lex);

// tokenization
char	*scan_token(t_lexer *lex);
char	*metachar_handle(t_lexer *lex);
char	*string_handle(t_lexer *lex);
char	*redirect_handle(t_lexer *lex);
char	*poststring_handle(t_lexer *lex, char *lhs, int i);
char	*variable_handle(t_lexer *lex);
char	*slash_handle(t_lexer *lex);
char	*single_quotes_handle(t_lexer *lex);
char	*double_quotes_handle(t_lexer *lex);
int	  default_handle(t_lexer *lex, const char *value, e_token type);

// string utils
size_t	ft_strncpy(char *d, const char *s, size_t n);
char    *get_word(const char *s, size_t n);
char    *merge_str(char *s1, char *s2);
int   	is_metachar(char ch);
int	    is_redirectchar(char ch);
int	    is_catchar(char ch);

// expand tokens and build tree
t_parser    *new_parser(t_lexer *lex);
t_var	      *new_tvar(const t_word_list *token);
t_cmd	      *assignment_handle(t_parser *parser, t_cmd *root);
t_cmd	      *add_tnode(t_cmd *left_node, t_cmd *right_node, int type);
t_cmd	      *parse_token(t_parser *parser);
int         add_field_argv(t_list *args, t_exec_cmd *cmd);
t_cmd	      *add_tnode(t_cmd *left_node, t_cmd *right_node, int type);
t_cmd	      *build_tree(t_parser *parser);
t_cmd	      *build_subtree(t_parser *parser);
t_cmd       *build_AST(t_parser *parser);

// parse utils
int	        is_redirect(e_token type);
int	        is_cmd_delimeter(e_token type);
void	      free_arr(char **arr);
char	      **get_path(char *path_env);
char	      *parsing_path(char **path_env, char *cmd_name);
t_cmd	      *new_exec_cmd(void);
int	        add_field_fname(e_token redirect_type, char *fname, t_exec_cmd *cmd);
int	        add_field_open_mode(e_token redirect_type, t_exec_cmd *cmd);
int	        free_cmd(t_exec_cmd *cmd);

// debug
const char  *get_type(int type);
//void        print_tree(t_ast *root);
void          print_tree(t_ast* root);


#endif