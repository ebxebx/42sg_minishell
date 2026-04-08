/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 11:26:08 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 15:34:46 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"
#include "executor.h"

static void	exit_heredoc_child(t_shell *shell, int status)
{
	rl_clear_history();
	if (shell)
		free_shell(shell);
	close_all_fds();
	exit(status);
}

static int	fork_and_exec_heredoc(t_shell *shell, char *limiter, char *path,
		int should_expand)
{
	pid_t	pid;
	int		child_status;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		g_signal = 0;
		init_signal_heredoc();
		child_status = read_heredoc_to_path(shell, limiter, path,
				should_expand);
		free(limiter);
		free(path);
		exit_heredoc_child(shell, child_status);
	}
	return (pid);
}

int	run_heredoc_child(t_shell *shell, char *limiter, char *path,
		int should_expand)
{
	pid_t	pid;
	int		status;

	pid = fork_and_exec_heredoc(shell, limiter, path, should_expand);
	signal(SIGINT, SIG_IGN);
	while (waitpid(pid, &status, 0) < 0)
	{
		if (errno != EINTR)
			break ;
	}
	init_signal_prompt();
	if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			g_signal = SIGINT;
		return (unlink(path), 1);
	}
	return (0);
}
