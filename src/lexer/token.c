/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:15:55 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/20 10:15:25 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_list	*new_token_list(t_list *env, char *input)
{
	t_list	*cmd_tokens;
	t_lexer	*lex;

	if (!input)
		return (NULL);
	lex = new_lexer(input, env);
	if (!lex)
		return (NULL);
	start_tokenization(lex);
	cmd_tokens = lex->tokens;
	free(lex);
	return (cmd_tokens);
}

t_lexer	*new_lexer(char *input, t_list *env_list)
{
	t_lexer	*lex;

	if (!input)
		return (NULL);
	lex = (t_lexer *)ft_calloc(1, sizeof(t_lexer));
	if (!lex)
		return (NULL);
	lex->input = input;
	lex->str_pos = input;
	lex->env = env_list;
	return (lex);
}

void	push_token(t_list **token_list, char *new_word, e_token type)
{
	t_token	*cur_token_pos;
	t_list	*new_lst_node;

	cur_token_pos = new_token(new_word, type);
	new_lst_node = ft_lstnew(cur_token_pos);
	ft_lstadd_back(token_list, new_lst_node);
}

t_token	*new_token(char *word, e_token type)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_list));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->word = word;
	return (new_token);
}

void	free_token(void *cur_token_pos)
{
	free(((t_token *)cur_token_pos)->word);
	free(cur_token_pos);
}
