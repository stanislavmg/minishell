#include "parser.h"
#define	ERR_SYNTAX "minishell: syntax error near unexpected token `"

t_parser *new_parser(t_lexer *lex)
{
	t_parser *pars_info;

	pars_info = (t_parser *)ft_calloc(1, sizeof(t_parser));
	if (!pars_info)
		return (NULL);
	pars_info->token = lex->tokens;
	return (pars_info);
}

int assignment_var(t_lexer *lex)
{
	t_word_list *token;

	token = lex->token_pos;
	while (token && !is_cmd_delimeter(token->type))
	{
		token = token->next;
	}
}

int	check_syntax(t_lexer *lex)
{
	t_word_list	*tokens;
	t_syntax	*token_info;

	tokens = lex->tokens;
	while (tokens)
	{
		assignment_var();
		if (is_cmd_delimeter(tokens->type))
		{
			printf(ERR_SYNTAX, "%s", tokens->word);
			lex->err = ERR_SYNTAX;
			return (1);
		}
		tokens = tokens->next;
	}
	free(token_info);
	return (0);
}