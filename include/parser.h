/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:31 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/09 14:37:47 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "types.h"
# include "env.h"
# include "lexer.h"
# include "error.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>

/* here document and helpers */
char		*new_var_name(char *str);
int			is_var_delimeter(int ch);
void	    print_msh_err(char *token_name);
e_token	    get_token_type(t_parser *parser);
char		*here_doc_start(char *stop_word, t_list *env, e_token mode);
void	    remove_duplicate(t_list **redirects);
void    	free_array(char **arr);

/* parse command arguments */
char	    **get_path(char *path_env);
char	    *parsing_path(char **path_env, char *cmd_name);
t_exec_cmd	*new_exec_cmd(t_list *args);
char	    **add_field_argv(t_list *args);

/* parse command subtree */
t_cmd	    *new_cmd_tree(t_parser *parser);
void	    push_variable(t_list **var_lst, t_token *token);
void	    push_redirect(t_list **redir, t_parser *parser);
void	    push_arg(t_list **args, t_parser *parser);

/* core parser utils */
t_cmd	    *new_ast(t_parser *parser);
t_cmd	    *build_tree(t_parser *parser);
t_cmd	    *parse_block(t_parser *parser);
t_cmd	    *parse_line(t_parser *parser);
t_cmd	    *parse_redirect(e_token redir_type, char *fname, t_list *env);
t_cmd	    *build_tree_fromlist(t_list *nodes, e_token type);
int	        read_new_input(t_parser *parser);
t_cmd	    *parse_cmd(t_list *var, t_list *args, t_list *redir);

/* parser and ast constructors */
t_redir	    *new_redir(e_token redirect_type, char *fname);
t_parser	*new_parser(t_list *tokens, t_list *env);
t_var	    *new_tvar(const char *key_and_value);
t_cmd	    *add_ast_node(t_cmd *left_node, t_cmd *right_node, int type);

/* debug */
const char *get_type(int type);
void	print_tokens(t_list *tokens);
void print_tree(t_ast* root);
#endif