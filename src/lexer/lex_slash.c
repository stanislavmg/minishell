#include "parser.h"
char	*get_hd_stop_word(t_lexer *lex)
{
	int		i;
	char	*stop_word;
	char	qoutes;

	i = 0;
	qoutes = 0;
	stop_word = NULL;
	while (ft_isspace(*lex->str_pos))
		lex->str_pos++;
	if (*lex->str_pos == '\'' || *lex->str_pos == '\"')
	{
		qoutes = *lex->str_pos;
		lex->str_pos++;
	}
	while (!is_token_delimeter(lex->str_pos[i]) && lex->str_pos[i] != qoutes)
		i++;
	if (qoutes && lex->str_pos[i] != qoutes)
		lex->err = ERR_SYNTAX;
	if (i)
		stop_word = get_word(lex->str_pos, i);
	lex->str_pos += i;
	if (qoutes)
		lex->str_pos++;
	if (*lex->str_pos && !ft_isspace(*lex->str_pos)
		&& !is_metachar(*lex->str_pos))
		stop_word = merge_str(stop_word, get_hd_stop_word(lex));
	return (stop_word);
}

char	*slash_handle(t_lexer *lex)
{
	char	*new_word;

	lex->str_pos++;
	if (lex->in_qoutes)
	{
		if (is_screening_ch(*lex->str_pos))
		{
			new_word = get_word(lex->str_pos, 1);
			lex->str_pos++;
		}
		else
			new_word = ft_strdup("\\");
		new_word = merge_str(new_word, double_quotes_handle(lex));
	}
	else
	{
		new_word = get_word(lex->str_pos, 1);
		if (*lex->str_pos)
			lex->str_pos++;
	}
	if (is_catchar(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}
