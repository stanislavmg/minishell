#include "parser.h"

int	is_cmd_delimeter(e_token type)
{ 
	return (AND == type || PIPE == type ||
			OR == type || SEMICOLON == type);
}

int	is_redirect(e_token type)
{
	return (HERE_DOC == type || INPUT_TRUNC == type ||
			OUTPUT_ADD == type || OUTPUT_TRUNC == type);
}

char *get_var_name(const char *token_word)
{
	char	*var_name;
	char	*end_var_name;
	size_t	len_var_name;

	var_name = NULL;
	end_var_name = ft_strchr(token_word, '=');
	if (!end_var_name)
		return (NULL);
	len_var_name = end_var_name - token_word;
	var_name = (char *)malloc(sizeof(char) * len_var_name + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, token_word, len_var_name + 1);
	return (var_name);
}

char *get_var_value(const char *token_word)
{
	char	*var_value;
	char	*start_var_value;
	size_t	len_var_value;

	var_value = NULL;
	start_var_value = ft_strchr(token_word, '=') + 1;
	len_var_value = ft_strlen(start_var_value);
	var_value = (char *)malloc(sizeof(char) * len_var_value + 1);
	if (!var_value)
		return (NULL);
	ft_strlcpy(var_value, start_var_value, len_var_value + 1);
	return (var_value);
}