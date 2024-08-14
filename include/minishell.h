<<<<<<< HEAD
#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>

=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:25 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/10 17:44:51 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHEL_H
#define MINISHEL_H

# define PROMT	"minishell$> "

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

typedef enum t_token
{
  END,
  COMMAND,
  REDIRECT,
  IO_FILE,
  STRING,
  VARIABLE,
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

typedef struct s_env
{
	char	*key;
	char	*value;
	int 	attr;
} 	t_env;

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

typedef struct s_redir
{
  e_token type;
  int     mode;
  char    *fname;
} t_redir;

typedef struct s_var
{
    e_token type;
    char    *key;
    char    *value;
}   t_var;

typedef struct s_exec_cmd
{
    e_token type;
    char    *path;
    char    **argv;
}   t_exec_cmd;

typedef struct s_token
{
  e_token type;
  char    *word;
} t_token;

typedef struct s_parser
{
  t_list      *cur_token_pos;
  t_ast       *cmd_tree;
  t_list      *env_lst;
  char        **paths;
  int         cmd_start;
  int         brackets_count;
  int         err;
} t_parser;

typedef struct s_lexer
{
  t_list      *tokens;
  t_list      *env;
  char        *input_str;
  char        *str_pos;
  int         in_qoutes;
  int         err;
} t_lexer;

typedef struct s_data
{
	t_lexer	 *lex;
	t_parser *parser;
	t_ast 	 *root;
	t_list   *env_list;
  char     *input;
} t_data;

>>>>>>> origin/parser
#endif