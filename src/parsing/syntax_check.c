#include "parser.h"

t_parser *new_parser(t_lexer *lex)
{
	t_parser 	*parser;
	t_env		*path;

	path = list_search(lex->env, "PATH");
	parser = (t_parser *)ft_calloc(1, sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->cur_token_pos = lex->tokens;
	parser->paths = get_path(path->value);
	return (parser);
}