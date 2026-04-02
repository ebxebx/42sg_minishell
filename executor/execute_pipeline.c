/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/30 19:05:23 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	close_pipe(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

static pid_t	fork_pipe_side(t_shell *shell, t_ast *ast, int *pipe_fd, int is_left)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		if (is_left && dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			exit(1);
		}
		if (!is_left && dup2(pipe_fd[0], STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(1);
		}
		close_pipe(pipe_fd);
		execute_ast_child(shell, ast);
	}
	return (pid);
}

int	execute_pipeline(t_shell *shell, t_ast *ast)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		left_status;
	int		right_status;

	if (!shell || !ast || ft_strcmp(ast->value, "|"))
		return (1);
	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	left_pid = fork_pipe_side(shell, ast->left, pipe_fd, 1);
	if (left_pid < 0)
		return (close_pipe(pipe_fd), 1);
	right_pid = fork_pipe_side(shell, ast->right, pipe_fd, 0);
	if (right_pid < 0)
		return (close_pipe(pipe_fd), 1);
	close_pipe(pipe_fd);
	left_status = 0;
	right_status = 0;
	init_signal_exec();
	while (waitpid(left_pid, &left_status, 0) < 0)
	{
		if (errno != EINTR)
			return (init_signal_prompt(), perror("waitpid"), 1);
	}
	while (waitpid(right_pid, &right_status, 0) < 0)
	{
		if (errno != EINTR)
			return (init_signal_prompt(), perror("waitpid"), 1);
	}
	init_signal_prompt();
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	if (WIFSIGNALED(right_status))
		return (128 + WTERMSIG(right_status));
	return (1);
}
