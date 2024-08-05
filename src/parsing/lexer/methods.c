#include "parser.h"

t_lexer	*new_lexer(char *input, t_list *env_list)
{
	t_lexer *lex;

	if (!input)
		return (NULL);
	lex = (t_lexer *)ft_calloc(1, sizeof(t_lexer));
	if (!lex)
		return (NULL);
	lex->str_pos = input;
	lex->input_str = input;
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

void	error_handle(t_lexer *lex)
{
	if (ERR_MEM == lex->err)
		printf("minishell: No memory\n");
	else if (ERR_QUOTE == lex->err)
		printf("minishell: Quotes wasn\'t close\n");
	else if (ERR_TOKEN == lex->err)
		printf("minishell: syntax error near unexpected cur_token_pos `%c'\n", *lex->str_pos);
	else
		printf("minishell: Undefined error. Abort!\n");
}

void free_token(void *cur_token_pos)
{
	free(((t_token *)cur_token_pos)->word);
	free(cur_token_pos);
}

void	free_lexer(t_lexer *lex)
{
	//free(lex->input_str);
	ft_lstclear(&lex->tokens, free_token);
	free(lex);
}

int	init_list(t_lexer *lex)
{
	char	*new_word;

	new_word = NULL;
	while(*lex->str_pos)
	{
		new_word = scan_token(lex);
		if (lex->err)
		{
			error_handle(lex);
			//free_lexer(lex);
			return (1);
		}
		else if (new_word && *new_word) //FIXME leak when return ""
			push_token(&lex->tokens, new_word, STRING);
	}
	//push_token(&lex->tokens, NULL, END);
	return (0);
}

void	push_token(t_list **token_list, char *new_word, e_token type)
{
	t_token	*cur_token_pos;
	t_list	*new_lst_node;

	cur_token_pos = new_token(new_word, type);
	new_lst_node = ft_lstnew(cur_token_pos);
	ft_lstadd_back(token_list, new_lst_node);
}