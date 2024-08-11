#include "exec.h"
#include "minishell.h"

void	free_minishell(t_data *msh)
{
	free_lexer(msh->lex);
	free_parser(msh->parser);
	free_ast(msh->root);
}

t_data	*new_msh_data(void)
{
	t_data *msh;

	msh = (t_data *)ft_calloc(1, sizeof(t_data));
	if (!msh)
	{
		perror("minishell: ");
		exit(ERR_MEM);
	}
	return (msh);
}

int	init_msh_data(t_data *msh, char *input)
{
	msh->lex = new_lexer(input, msh->env_list);
	init_list(msh->lex);
	if (msh->lex->err)
	{
		print_msh_err(ft_lstlast(msh->lex->tokens)->data);
		return (2);
	}
	msh->parser = new_parser(msh->lex);
	msh->root = (t_ast *)new_ast(msh->parser);
	//print_tree(msh->root);
	if (msh->parser->err)
	{
		if (msh->parser->cur_token_pos)
			print_msh_err(msh->parser->cur_token_pos->data);
		else
			print_msh_err(ft_lstlast(msh->lex->tokens)->data);
		return (2);
	}
	return (0);
}

void	set_std_val(t_list *env)
{
	t_env	*n;
	t_env	*shell_lvl;
	int		tmp_value;
	
	if (!env)
		return ;
	n = new_env(get_var_name("?=0"), get_var_value("?=0"), HIDDEN);
	ft_lstadd_back(&env, ft_lstnew(n));
	shell_lvl = get_env(env, "SHLVL");
	if (!shell_lvl)
	{
		n = new_env(get_var_name("SHLVL=0"), get_var_value("SHLVL=1"), HIDDEN);
		ft_lstadd_back(&env, ft_lstnew(n));
	}
	else if (!*shell_lvl->value)
	{
		free(shell_lvl->value);
		shell_lvl->value = ft_strdup("1");
	}
	else
	{
		tmp_value = ft_atoi(shell_lvl->value) + 1;
		free(shell_lvl->value);
		shell_lvl->value = ft_itoa(tmp_value);
	}
}

int	main(int ac, char **av, char **env)
{
	t_data		*msh;
	
	(void)ac;
	(void)av;
	msh = new_msh_data();
	msh->env_list = new_env_list(env);
	set_std_val(msh->env_list);
	msh->input = readline(PROMT);
	while (msh->input)
	{
		add_history(msh->input);
		if (!strcmp(msh->input, "exit"))
			break ;
		if (!init_msh_data(msh, msh->input))
			travers_tree((t_ast *)msh->root, msh->env_list);
		free_minishell(msh);
		msh->input = readline(PROMT);
	}
	ft_lstclear(&msh->env_list, free_env);
	free(msh->input);
	free(msh);
	return (0);
}