/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:47:19 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/11 17:48:14 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include "libft.h"
#include "error.h"

extern int	g_exit_code;

static void	interrupt_execution(int sig)
{
	(void)sig;
	//write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	g_exit_code = FT_SIGINT;
}

static void	redisplay_promt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_code = 1;
}

void	set_redisplay_behavior(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	act.sa_handler = redisplay_promt;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
}

void	set_interrupt_behavior(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	act.sa_handler = interrupt_execution;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
}

void	setup_termios(void)
{
	struct termios	t;

	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
