#include "parser.h"

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	if (!arr)
		return ;
	while (arr[++i])
		free(arr[i]);
	free(arr);
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

t_var	*new_tvar(const t_word_list *token)
{
	t_var	*new_var;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
		return (NULL);
	new_var->key = get_var_name(token->word);
	new_var->value = get_var_value(token->word);
	return (new_var);
}

t_cmd	*assignment_handle(t_parser *parser, t_cmd *root)
{
	t_cmd		*new_tree;
	t_word_list *cur_token;

	if (!parser || !parser->token)
		return (NULL);
	new_tree = NULL;
	cur_token = parser->token;
	new_tree = new_tvar(cur_token);
	cur_token = cur_token->next;
	if (root)
		new_tree = add_tnode(root, new_tree, ASSIGNMENT);
	while (ft_strchr(cur_token->word, '='))
	{
		new_tree = add_tnode(new_tree, new_tvar(cur_token), ASSIGNMENT);
		cur_token = cur_token->next;
	}
	parser->token = cur_token;
	return (new_tree);
}

t_cmd	*parse_token(t_parser *parser)
{
	t_exec_cmd	*new_cmd;
	t_ast		*root;
	t_list		*args;
	t_word_list *cur_token;

	if (!parser || !parser->token)
		return (NULL);
	root = NULL;
	cur_token = parser->token;
	new_cmd = init_cmd(parser);
	if (!new_cmd)
	{
		parser->err = ERR_MEM;
		return (NULL);
	}
	while (cur_token && !is_cmd_delimeter(cur_token->type))
	{
		if (ft_strchr(cur_token->word, '=') && !parser->cmd_start)
			root = assignment_handle(parser, root);
		else if (is_redirect(cur_token->type))
		{
			if (cur_token->type == HERE_DOC)
				here_doc_start(); // TODO
			add_field_fnames(cur_token->type, cur_token->next->word, new_cmd);
			add_field_open_mode(cur_token->type, new_cmd);
			cur_token = cur_token->next;
			cur_token->word = NULL;
		}
		else if (cur_token->type == STRING && !parser->cmd_start)
		{
			parser->cmd_start = 1;
			ft_lstadd_back(&args, ft_lstnew(cur_token->word));
			cur_token->word = NULL;
		}
		cur_token = cur_token->next;
	}
	if (!parser->cmd_start)
	{
		parser->cmd_start = 0;
		add_field_argv(args, new_cmd);
		ft_lstclear(&args, free);
	}
	else
		free_cmd(new_cmd);
	return (new_cmd);
}

int	free_cmd(t_exec_cmd *cmd)
{
	free_arr(cmd->argv);
	free(cmd->argv);
	free(cmd->in_fname);
	free(cmd->out_fname);
	free(cmd);
	return (0);
}

int add_field_argv(t_list *args, t_exec_cmd *cmd)
{
	char	**argv;
	int		argc;
	int		i;

	if (!args || !cmd)
		return (1);
	i = -1;
	argc = ft_lstsize(args) + 1;
	argv = (char **)malloc(sizeof(char *) * argc);
	if (!argv)
		return (NULL);
	while (++i < argc)
	{
		argv[i] = (char *)args->data;
		args->data = NULL;
		args = args->next;
	}
	argv[i] = NULL;
	cmd->argv = argv;
	return (0);
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

t_cmd	*build_tree(t_parser *parser)
{
	t_cmd	*root;
	e_token	type;
	
	if (!parser->token)
		return (NULL);
	if (parser->token->type == OPEN_BRACKET)
		parser->brackets_count++;
	root = parse_token(parser);
	while (parser->token && parser->token->type != CLOSED_BRACKET)
	{
		type = parser->token->type;
		if (type == AND || type == OR || type == SEMICOLON)
		{
			parser->token = parser->token->next;
			root = add_tnode(root, build_subtree(parser), type);
		}
		else
			root = add_tnode(root, parse_token(parser), type);
	}
	return (root);
}

t_cmd	*build_subtree(t_parser *parser)
{
	e_token type;
	t_cmd	*root;
	
	root = NULL;
	if (parser->token->type == CLOSED_BRACKET)
		return (root);
	if (parser->token->type == OPEN_BRACKET)
	{
		parser->token = parser->token->next;
		root = add_tnode()
	}
	root = parse_token(parser);
	while (parser->token &&
			parser->token->type != AND &&
			parser->token->type != OR &&
			parser->token->type != SEMICOLON)
	{
		type = parser->token->type;
		root = add_tnode(root, parse_tokens(parser), type);
	}
	return (root);
}

t_cmd *build_AST(t_parser *parser)
{
	t_cmd		*root;

	root = build_tree(parser);
	if (!root)
		return (NULL);
	print_tree((t_ast *)root);
	return (root);
}
