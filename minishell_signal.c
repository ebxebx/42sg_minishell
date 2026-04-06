/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:29:50 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/06 18:24:58 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_signal = 0;

static void	sigint_prompt_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = signo;
	}
}

static void	sigint_exec_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		g_signal = signo;
	}
}

static void	sigint_heredoc_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_done = 1;
		g_signal = signo;
	}
}

void	init_signal_prompt(void)
{
	signal(SIGINT, sigint_prompt_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_signal_exec(void)
{
	signal(SIGINT, sigint_exec_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_signal_heredoc(void)
{
	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
}
