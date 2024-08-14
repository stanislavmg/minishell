#include "parser.h"

t_lexer	*new_lexer(char *input, t_list *env_list)
{
	t_lexer *lex;

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

t_token	*new_token(char *word, e_token type)
{
	t_token *new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_list));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->word = word;
	return (new_token);
}

void free_token(void *cur_token_pos)
{
	free(((t_token *)cur_token_pos)->word);
	free(cur_token_pos);
}

void add_new_input(t_lexer *lex)
{
	char	*new_input;
	int		cur_pos;

	new_input = NULL;
	cur_pos = ft_strlen(lex->input);
	new_input = readline("> ");
	lex->input = merge_str(lex->input, new_input);	
	if (!lex->input)
		lex->err = ERR_MEM;
	lex->str_pos = lex->input + cur_pos;
}


void start_tokenization(t_lexer *lex)
{
	char *new_word;

	new_word = NULL;
	while(*lex->str_pos && !lex->err)
	{
		new_word = scan_token(lex);
		if (new_word && !*new_word)
			free(new_word);
		else if (new_word)
			push_token(&lex->tokens, new_word, STRING);	
		if (!*lex->str_pos && ((t_token *)ft_lstlast(lex->tokens)->data)->type == PIPE)// FIXME when input contain only spaces segfault
			add_new_input(lex);
	}
	if (lex->err)
	{
		print_msh_err(ft_lstlast(lex->tokens)->data);
		ft_lstclear(&lex->tokens, free_token);
	}
	add_history(lex->input);
	free(lex->input);
}

t_list	*new_token_list(t_list *env, char *input)
{
	t_list	*cmd_tokens;
	t_lexer *lex;

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

void	push_token(t_list **token_list, char *new_word, e_token type)
{
	t_token	*cur_token_pos;
	t_list	*new_lst_node;

	cur_token_pos = new_token(new_word, type);
	new_lst_node = ft_lstnew(cur_token_pos);
	ft_lstadd_back(token_list, new_lst_node);
}