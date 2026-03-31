/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:29:50 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/31 20:42:48 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_signal = 0;

static void	sigint_handler(int signo)
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

static void	sigquit_handler(int signo)
{
	if (signo == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 20);
		g_signal = signo;
	}
}

void	init_signal(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}
