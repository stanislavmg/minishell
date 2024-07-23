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

int	insert_variable(t_word_list *token, t_env *list_env)
{
	char **new_var;
	t_env	*search_var;

	new_var = split_var(token->word);
	if (!new_var)
		return (1);
	search_var = list_search(list_env, new_var[0]);
	if (!search_var)
		list_add(&list_env, list_new(new_var[0], new_var[1]));
	else
	{
		free(search_var->value);
		search_var->value = new_var[0];
		new_var[0] = NULL;
		free(new_var[1]);
		free(new_var);
	}
	return (0);
}

void	parse_variable(t_lexer *lex)
{

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

	if (!lex->token_pos)
		return (NULL);
	if (is_cmd_delimeter(lex->token_pos->type))
		lex->token_pos = lex->token_pos->next;
	while (ft_strchr(lex->token_pos->word, '='))
	{
		insert_variable(lex->token_pos, lse->env);
		lex->token_pos = lex->token_pos->next;
	}
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
	add_field_files(lex->token_pos, new_cmd);
	new_cmd->path = 
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
	t_ast	*root;

	root = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!root)
		return (NULL);
	root->left = left_node;
	root->right = right_node;
	root->type = type;
	return ((t_cmd *)root);
}

t_cmd	*build_tree(t_lexer *lex)
{
	t_cmd	*root;
	e_token	type;
	
	if (!lex->token_pos)
		return (NULL);
	root = parse_tokens(lex);
	while (lex->token_pos)
	{
		type = lex->token_pos->type;
		if (type == AND || type == OR)
			root = add_tnode(root, build_subtree(lex, parse_tokens(lex)), type);
		else
			root = build_subtree(lex, root);
	}
	return (root);
}

t_cmd *build_subtree(t_lexer *lex, t_cmd *root)
{
	e_token type;
	
	while (lex->token_pos && lex->token_pos->type != AND && lex->token_pos->type != OR)
	{
		type = lex->token_pos->type;
		root = add_tnode(root, parse_tokens(lex), type);
	}
	return (root);
}

t_cmd *build_AST(t_lexer *lex)
{
	t_cmd		*root;

	root = NULL;
	if (check_syntax(lex))
		return (NULL);
	root = build_tree(lex);
	print_tree((t_ast *)root);
	return (root);
}
