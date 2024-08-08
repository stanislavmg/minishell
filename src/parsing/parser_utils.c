#include "parser.h"

t_redir *new_redir(e_token redirect_type, char *fname)
{
	t_redir *new_node;

	new_node = (t_redir *)malloc(sizeof(t_redir));
	new_node->type = IO_FILE;
	new_node->fname = fname;
	if (OUTPUT_TRUNC == redirect_type)
		new_node->mode = O_TRUNC | O_CREAT | O_RDWR;
	else if (OUTPUT_ADD == redirect_type)
		new_node->mode = O_APPEND | O_CREAT | O_RDWR;
	else if (INPUT_TRUNC == redirect_type)
		new_node->mode = O_RDONLY;
	else
		new_node->mode = 0;
	return (new_node);
}

t_parser *new_parser(t_lexer *lex)
{
	t_parser 	*parser;
	t_env		*path;

	path = get_env(lex->env, "PATH");
	parser = (t_parser *)ft_calloc(1, sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->cur_token_pos = lex->tokens;
	parser->paths = get_path(path->value);
	return (parser);
}