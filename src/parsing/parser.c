#include "../../inc/parser.h"

int	check_syntax(t_lexer *lex)
{
	t_word_list *tokens;

	tokens = lex->tokens;
	while (tokens)
	{
		if (is_cmd_delimeter(tokens->type) && is_cmd_delimeter(tokens->next->type))
		{
			lex->err = ERR_SYNTAX;
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

char **split_var(char *str)
{
	char	**res;
	char	*start_value;
	int		len_value;
	int		len_name;

	res = (char **)malloc(sizeof(char *) * 3);
	if (!res)
		return (NULL);
	start_value = ft_strchr(str, '=');
	len_value = ft_strlen(start_value + 1);
	len_name = start_value - str;
	res[0] = (char *)malloc(sizeof(char) * len_name + 1);
	if (!res[0])
	{
		free(res);
		return (NULL);
	}
	res[1] = (char *)malloc(sizeof(char) * len_value + 1);
	if (!res[1])
	{
		free(res[0]);
		free(res);
		return (NULL);
	}
	ft_strncpy(res[0], str, len_name);
	ft_strncpy(res[1], start_value + 1, len_value);
	res[2] = NULL;
	return (res);
}

int	insert_variable(t_lexer *lex, t_word_list *token)
{
	char **new_var;

	new_var = split_var(token->word);
	if (!new_var)
	{
		lex->err = ERR_MEM;
		return (1);
	}
	//add_var(); // TO DO function which insert var in one-link list
	return (0);
}

void	parse_variable(t_lexer *lex)
{
	// if (new_cmd && ft_strchr(cur_token->word, '='))
	// {
	// 	insert_variable(lex, cur_token);
	// 	lex->token_pos = lex->token_pos->next;
	// }
}

int	count_args(t_word_list *tokens)
{
	int	argc;
	
	argc = 0;
	while (tokens && !is_cmd_delimeter(tokens->type))
	{
		if (is_redirect(tokens->type))
			tokens = tokens->next;
		else if (tokens->type == STRING)
			argc++;
		tokens = tokens->next;
	}
	return (argc);
}

t_cmd	*parse_tokens(t_lexer *lex)
{
	t_cmd *new_cmd;

	new_cmd = (t_cmd *)init_cmd(lex);
	if (!new_cmd)
		return (NULL);
	while (lex->token_pos && !is_cmd_delimeter(lex->token_pos->type))
		lex->token_pos = lex->token_pos->next;
	return (new_cmd);
}

t_exec_cmd	*init_cmd(t_lexer *lex)
{
	int			argc;
	t_exec_cmd	*new_cmd;

	if (!lex->token_pos)
		return (NULL);
	new_cmd = (t_exec_cmd *)create_cmd();
	if (!new_cmd)
	{
		lex->err = ERR_MEM;
		return (NULL);
	}
	argc = count_args(lex->token_pos);
	new_cmd->argv = add_field_argv(lex->token_pos, argc);
	add_field_fd(lex->token_pos, new_cmd);
	return (new_cmd);
}

char	**add_field_argv(t_word_list *tokens, int argc)
{
	char	**argv;
	int		i;

	i = 0;
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	while (tokens && !is_cmd_delimeter(tokens->type))
	{
		argv[i] = tokens->word;
		tokens->word = NULL;
		tokens = tokens->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

t_cmd	*add_tnode(t_cmd *left_node, t_cmd *right_node, int type)
{
	t_pipecmd	*root;

	root = (t_pipecmd *)ft_calloc(1, sizeof(t_pipecmd));
	if (!root)
		return (NULL);
	root->left = left_node;
	root->right = right_node;
	root->type = type;
	return ((t_cmd *)root);
}


t_cmd *build_tree(t_lexer *lex)
{
	t_cmd		*new_cmd;
	t_pipecmd	*root;

	root = NULL;
	if (!lex->token_pos)
		return (NULL);
	new_cmd = parse_tokens(lex);
	while (lex->token_pos)
	{
	if (lex->token_pos->type == AND || lex->token_pos->type == OR)
	{
		lex->token_pos = lex->token_pos->next;
		root = (t_pipecmd *)add_tnode(new_cmd, build_tree(lex), lex->token_pos->type);
		return ((t_cmd *)root);
	}
	else
	{
		lex->token_pos = lex->token_pos->next;
	}
	}
	root = (t_pipecmd *)add_tnode(root, parse_tokens(lex), lex->token_pos->type);
	// while (lex->token_pos)
	// {
	// 	lex->token_pos = lex->token_pos->next;
	// 	root = (t_pipecmd *)add_tnode((t_cmd *)root, build_tree(lex), lex->token_pos->type);
	// }
	return ((t_cmd *)root);
}

t_cmd *build_AST(t_lexer *lex)
{
	t_cmd		*root;

	root = NULL;
	if (check_syntax(lex))
		return (NULL);
	root = build_tree(lex);
	print_tree((t_pipecmd *)root);
	return (root);
}
