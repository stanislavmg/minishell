#include "../../inc/parser.h"

size_t	ft_strncpy(char *d, const char *s, size_t n)
{
	size_t	i;

	i = 0;
    if (!d || !s || n == 0)
        return 0;
    while (i < n && s[i])
	{
        d[i] = s[i];
        i++;
    }
    d[i] = 0;
	return (i);
}

char *merge_str(char *s1, char *s2)
{
	char	*res;

    if (!s1 || !s2)
        return (NULL);
    res = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return (res);
}

char *get_word(const char *s, size_t n)
{
	char *new_word;

	if (!s)
		return (NULL);
	new_word = (char *)malloc(n + 1);
	if (!new_word)
		return (NULL);
	if (n == 0)
	{
		*new_word = 0;
		return (new_word);
	}
	ft_strncpy(new_word, s, n);
	return (new_word);
}

int	is_metachar(char ch)
{
	return ('|' == ch || '&' == ch || ';' == ch ||
			'<' == ch || '>' == ch); // += ( and )
}

int	is_catchar(char ch)
{
	return ('$' == ch || '\"' == ch || '\'' == ch ||
			'\\' == ch);
}