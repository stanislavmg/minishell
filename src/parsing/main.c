#include "exec.h"
#include "minishell.h"
#include <time.h>
#include <sys/time.h>

struct timeval start, end;
extern int g_exit_code;

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

t_cmd	*init_msh_data(t_list *env, char *input)
{
	t_list 		*tokens;
	t_parser	*parser;
	t_cmd		*ast;

	tokens = new_token_list(env, input);
	if (!tokens)
		return (NULL);
	//print_tokens(tokens);
	parser = new_parser(tokens, env);
	ast = new_ast(parser);
	//print_tree((t_ast *)ast);
	if (parser->err)
	{
		if (parser->cur_token_pos)
			print_msh_err(parser->cur_token_pos->data);
		else
			print_msh_err(ft_lstlast(tokens)->data);
		free_ast((t_ast *)ast);
		ast = NULL;
	}
	ft_lstclear(&tokens, free_token);
	free(parser);
	return (ast);
}

void	set_std_val(t_list *env)
{
	t_env	*n;
	t_env	*shell_lvl;
	int		tmp_value;
	
	if (!env)
		return ;
	/* FIXME violation of encapsulation */
	g_exit_code = 0;
	n = new_env(get_var_name("?=0"), get_var_value("?=0"), HIDDEN);
	ft_lstadd_back(&env, ft_lstnew(n));
	shell_lvl = get_env(env, "SHLVL");
	if (!shell_lvl)
	{
		n = new_env(get_var_name("SHLVL=0"), get_var_value("SHLVL=1"), ENV | EXPORT);
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
	/* OLDPWDRESET */
}

void one_arg_exec(t_data *msh, char **av)
{
	char 		*input;
	int			exit_code;
	
	exit_code = 0;
	input = ft_strdup(av[1]);
	msh->root = (t_ast *)init_msh_data(msh->env, input);
	if (msh->root)
		travers_tree((t_ast *)msh->root, msh);
	exit_code = get_exit_code();
	ft_lstclear(&msh->env, free_env);
	free_ast(msh->root);
	free(msh);
	exit(exit_code);
}

int	main(int ac, char **av, char **env)
{
	t_data		*msh;
	char 		*input;
	
	msh = new_msh_data();
	msh->env = new_env_list(env);
	set_std_val(msh->env);
	init_signals(0);
	init_signals(1);
	remove_echo_ctl();
	// if (ac > 1)
	// 	one_arg_exec(msh, av);
	input = readline(PROMT);
	while (input)
	{
		msh->root = (t_ast *)init_msh_data(msh->env, input);
		if (msh->root)
			travers_tree((t_ast *)msh->root, msh);
		free_ast(msh->root);
		while (msh->child_ps)
			ft_waitpid(msh);
		input = readline(PROMT);
	}
	ft_lstclear(&msh->env, free_env);
	free(input);
	free(msh);
	return (g_exit_code);
}