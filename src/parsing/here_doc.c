# include "parser.h"

char *here_doc_start(t_token *hd_token)
{
	int		i;
	int		ch;
	int		out_fd;
	int		stop_word_len;
	char	*hidden_fname;
	char	*stop_word;
	char	buf[1024];

	hidden_fname = NULL;
	if (!hd_token || hd_token->type != HERE_DOC)
		return (NULL);
	stop_word = hd_token->word;
	stop_word_len = (ft_strlen(stop_word));
	hidden_fname = ft_strjoin(".", hd_token->word);
	out_fd = ft_open(hidden_fname, O_RDWR | O_CREAT | O_TRUNC);
	while ((ch = read(0, buf, 1024)) > 0)
	{
		i = -1;
		while (++i < ch)
		{
			
		}
		write(out_fd, buf, ch);
	}
	ft_close(out_fd);
	return (hidden_fname);
}