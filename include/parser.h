/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:31 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/14 15:48:44 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "env.h"
# include "error.h"
# include "minishell.h"
# include <dirent.h>
# include <fcntl.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

// lexer methods
t_lexer	*new_lexer(char *input, t_list *env_list);
t_token	*new_token(char *word, e_token type);
void	push_token(t_list **token_list, char *new_word, e_token type);
t_list	*new_token_list(t_list *env, char *input);
void free_token(void *cur_token_pos);

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
int	  	default_handle(t_lexer *lex, const char *value, e_token type);

// string utils
size_t	ft_strncpy(char *d, const char *s, size_t n);
char    *get_word(const char *s, size_t n);
char    *merge_str(char *s1, char *s2);
int   	is_metachar(char ch);
int	    is_redirectchar(char ch);
int	    is_catchar(char ch);

// expand tokens and build tree
t_var		*new_tvar(const char *key_and_value);
t_cmd		*build_tree_fromlist(t_list *nodes, e_token type);
t_cmd		*add_tnode(t_cmd *left_node, t_cmd *right_node, int type);
t_cmd		*parse_line(t_parser *parser);
char		**add_field_argv(t_list *args);
t_exec_cmd	*new_exec_cmd(t_list *args);
t_cmd		*add_tnode(t_cmd *left_node, t_cmd *right_node, int type);
t_cmd		*build_tree(t_parser *parser);
t_cmd		*new_ast(t_parser *parser);
int			 here_doc_start(t_token *hd_token, t_list *env);
t_cmd 		*parse_redirect(t_token *token, t_list *env);

// parse utils
int			is_redirect(e_token type);
int			is_cmd_delimeter(e_token type);
void		free_arr(char **arr);
char		**get_path(char *path_env);
char		*parsing_path(char **path_env, char *cmd_name);
t_exec_cmd	*new_exec_cmd(t_list *args);
int			free_cmd(t_exec_cmd *cmd);
e_token		get_token_type(t_parser *parser);
t_redir		*new_redir(e_token redirect_type, char *fname);
void		add_variable_node(t_list **var_lst, t_token *token);
t_cmd 		*parse_cmd(t_list *var, t_list *args, t_list *redir);
t_cmd		*parse_block(t_parser *parser);
void		print_msh_err(t_token *token);
t_parser *new_parser(t_list *tokens, t_list *env);

// free
void	free_ast(t_ast *root);
void	free_arr(char **arr);
int		free_cmd(t_exec_cmd *cmd);

// debug
const char	*get_type(int type);
void		print_tree(t_ast* root);
void		print_tokens(t_list *tokens);

#endif