/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 21:13:44 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 10:52:27 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H

# define LEXER_H
# include "types.h"

/* tokenizing */
void	start_tokenization(t_lexer *lex);
char	*scan_token(t_lexer *lex);
int		default_handle(t_lexer *lex, const char *value, t_token_type type);
char	*redirect_handle(t_lexer *lex);
char	*metachar_handle(t_lexer *lex);
char	*double_quotes_handle(t_lexer *lex);
char	*string_handle(t_lexer *lex);
char	*single_quotes_handle(t_lexer *lex);
char	*get_hd_stop_word(t_lexer *lex, char *stop);
char	*slash_handle(t_lexer *lex);
char	*variable_handle(t_lexer *lex);

/* helpers */
int		is_cmd_delimeter(t_token_type type);
int		is_redirect(t_token_type type);
int		is_metachar(char ch);
int		is_catchar(char ch);
int		is_token_delimeter(int ch);
int		string_is_spaces(const char *str);
void	add_new_input(t_lexer *lex);
size_t	ft_strncpy(char *d, const char *s, size_t n);
char	*merge_str(char *s1, char *s2);
char	*get_word(const char *s, size_t n);

/* lexer methods */
t_list	*new_token_list(t_list *env, char *input);
t_lexer	*new_lexer(char *input, t_list *env_list);
t_token	*new_token(char *word, t_token_type type);
void	fret_token(void *cur_token_pos);
void	push_token(t_list **token_list, char *new_word, t_token_type type);

/* wildcard */
int		ft_fnmatch(char *pattern, char *filename, char *star_pos, char *match);
void	expand_wildcard(t_lexer *lex, char *pattern);

#endif