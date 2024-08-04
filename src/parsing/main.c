#include "exec.h"
#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_lexer 	*lex;
	t_ast		*root;
	t_env		*env_lst;
	t_list 		*list;
	t_parser	*parser;
	t_exec_data	*data;
	char 		*res;
	int 		i;
	char 		*s = "minishell$> ";
	t_env		*n = list_new(get_var_name("?=0"), get_var_value("?=0"));

	env_lst = create_envlist(env);
	list_add(&env_lst, n);
	data = new_exec_data(env, env_lst);
	while (1)
	{
		res = readline(s);
		if (!strcmp(res, "exit"))
			break ;
		lex = new_lexer(res, env_lst);
		if (init_list(lex))
			continue;
		list = lex->tokens;
		i = 0;
		while (list)
		{
			printf("#%d cur_token_pos: %s\ntype: %s\n\n", i, ((t_token*)list->data)->word, get_type(((t_token*)list->data)->type));
			i++;
			list = list->next;
		}
		parser = new_parser(lex);
		root = build_AST(parser);
		print_tree((t_ast *)root);
		travers_tree(root, data);
		free_lexer(lex);
		free_parser(parser); 
		free_tree(root);
	}
	return (0);
}