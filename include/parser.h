/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:31 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/31 11:19:23 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "env.h"
# include "error.h"
# include "types.h"
# include <dirent.h>
# include <fcntl.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

// lexer methods
t_lexer	*new_lexer(char *input, t_list *env_list);
t_token	*new_token(char *word, e_token type);
void	push_token(t_list **token_list, char *new_word, e_token type);
t_list	*new_token_list(t_list *env, char *input);
void 	free_token(void *cur_token_pos);

// tokenization
void	start_tokenization(t_lexer *lex);
char	*scan_token(t_lexer *lex);
char	*metachar_handle(t_lexer *lex);
char	*string_handle(t_lexer *lex);
char	*redirect_handle(t_lexer *lex);
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
int		string_is_spaces(const char *str);

// expand tokens and build tree
t_var		*new_tvar(const char *key_and_value);
t_cmd		*build_tree_fromlist(t_list *nodes, e_token type);
t_cmd		*add_ast_node(t_cmd *left_node, t_cmd *right_node, int type);
t_cmd		*parse_line(t_parser *parser);
char		**add_field_argv(t_list *args);
t_exec_cmd	*new_exec_cmd(t_list *args);
t_cmd		*build_tree(t_parser *parser);
t_cmd		*new_ast(t_parser *parser);
char		*here_doc_start(char *stop_word, t_list *env);
t_cmd		*parse_redirect(e_token redir_type, char *fname, t_list *env);
void		expand_wildcard(t_lexer *lex, char *pattern);

// parse utils
void	push_variable(t_list **var_lst, t_token *token);
void	push_arg(t_list **args, t_parser *parser);
void	push_redirect(t_list **redir, t_parser *parser);
void	remove_duplicate(t_list **redirects);
int			is_redirect(e_token type);
int			is_cmd_delimeter(e_token type);
void		free_arr(char **arr);
char		**get_path(char *path_env);
char		*parsing_path(char **path_env, char *cmd_name);
t_exec_cmd	*new_exec_cmd(t_list *args);
int			free_cmd(t_exec_cmd *cmd);
e_token		get_token_type(t_parser *parser);
t_redir		*new_redir(e_token redirect_type, char *fname);
t_cmd 		*parse_cmd(t_list *var, t_list *args, t_list *redir);
t_cmd		*parse_block(t_parser *parser);
void		print_msh_err(char *token_name);
t_parser 	*new_parser(t_list *tokens, t_list *env);
t_cmd	*new_cmd_tree(t_parser *parser);
int	is_token_delimeter(int ch);
int	is_screening_ch(int ch);
int	is_var_delimeter(int ch);
char	*get_hd_stop_word(t_lexer *lex);

// free
void	free_ast(t_ast *root);
void	free_arr(char **arr);
int		free_cmd(t_exec_cmd *cmd);
void	kill_child(t_list *ps);

// debug
const char	*get_type(int type);
void		print_tree(t_ast* root);
void		print_tokens(t_list *tokens);
int	fd_is_pipe(int fd);
#endif