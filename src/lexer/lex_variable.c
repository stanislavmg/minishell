
char	*variable_handle(t_lexer *lex)
{
	int		i;
	char	*new_word;
	t_env	*var;

	i = 0;
	var = NULL;
	new_word = NULL;
	lex->str_pos++;
	if (*lex->str_pos == '?')
	{
		new_word = ft_strdup("?");
		i = 1;
	}
	else
	{
		while (!is_var_delimeter(lex->str_pos[i])
			&& !is_token_delimeter(lex->str_pos[i])
			&& !is_catchar(lex->str_pos[i]))
			i++;
		if (i == 0)
			new_word = get_word("$", 1);
		else
			new_word = get_word(lex->str_pos, i);
	}
	lex->str_pos += i;
	if (i)
	{
		var = get_env(lex->env, new_word);
		free(new_word);
		if (var)
			new_word = ft_strdup(var->value);
		else
			new_word = NULL;
	}
	if (lex->in_qoutes)
		new_word = merge_str(new_word, double_quotes_handle(lex));
	else if (is_catchar(*lex->str_pos) || !ft_isspace(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}