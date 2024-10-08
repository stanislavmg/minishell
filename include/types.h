/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:25 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/22 17:47:08 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H 

typedef struct s_list	t_list;

typedef enum e_token
{
	END,
	COMMAND,
	REDIRECT,
	IO_FILE,
	ARG,
	VARIABLE,
	INPUT_TRUNC,
	ASSIGNMENT,
	OUTPUT_ADD,
	OUTPUT_TRUNC,
	AND,
	PIPE,
	OR,
	SEMICOLON,
	HERE_DOC,
	EXPAND_HERE_DOC,
	OPEN_BRACKET,
	CLOSED_BRACKET
}	t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				attr;
}	t_env;

typedef struct s_cmd
{
	t_token_type	type;
}	t_cmd;

typedef struct s_ast
{
	t_token_type	type;
	t_cmd			*left;
	t_cmd			*right;
}	t_ast;

typedef struct s_redir
{
	t_token_type	type;
	int				mode;
	char			*fname;
}	t_redir;

typedef struct s_var
{
	t_token_type	type;
	char			*key;
	char			*value;
}	t_var;

typedef struct s_exec_cmd
{
	t_token_type	type;
	char			*path;
	char			**argv;
}	t_exec_cmd;

typedef struct s_token
{
	t_token_type	type;
	char			*word;
}	t_token;

typedef struct s_parser
{
	t_list			*cur_token_pos;
	t_list			*env_lst;
	int				cmd_start;
	int				brackets_count;
	int				err;
}	t_parser;

typedef struct s_lexer
{
	t_list			*tokens;
	t_list			*env;
	char			*input;
	char			*str_pos;
	int				in_qoutes;
	int				err;
}	t_lexer;

typedef struct s_data
{
	t_ast			*root;
	t_list			*env;
	t_list			*child_ps;
}	t_data;

#endif