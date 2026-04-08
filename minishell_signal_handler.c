/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 18:34:17 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 18:34:49 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_prompt_handler(int signo)
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

void	sigint_exec_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		g_signal = signo;
	}
}

/* Break the child's blocking readline() immediately on Ctrl-C. */
void	sigint_heredoc_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		g_signal = signo;
		close(STDIN_FILENO);
	}
}
