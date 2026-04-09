/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/09 14:46:06 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	close_pipe(int *pipe_fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

static pid_t	fork_pipe_side(t_shell *shell, t_ast *ast, int *pipe_fd,
	int side)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		if (side == LEFT && dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2");
			exit(1);
		}
		if (side == RIGHT && dup2(pipe_fd[0], STDIN_FILENO) < 0)
		{
			perror("dup2");
			exit(1);
		}
		close_pipe(pipe_fd);
		execute_ast_child(shell, ast);
	}
	return (pid);
}

static int	fork_pipeline_children(t_shell *shell, t_ast *ast, int *pipe_fd,
	pid_t pids[2])
{
	pids[LEFT] = fork_pipe_side(shell, ast->left, pipe_fd, LEFT);
	if (pids[LEFT] < 0)
		return (close_pipe(pipe_fd), 1);
	pids[RIGHT] = fork_pipe_side(shell, ast->right, pipe_fd, RIGHT);
	if (pids[RIGHT] < 0)
		return (close_pipe(pipe_fd), 1);
	return (0);
}

static int	wait_child_process(pid_t pid, int *status)
{
	while (waitpid(pid, status, 0) < 0)
	{
		if (errno != EINTR)
			return (perror("waitpid"), 1);
	}
	return (0);
}

int	execute_pipeline(t_shell *shell, t_ast *ast)
{
	int		pipe_fd[2];
	pid_t	pids[2];
	int		status[2];

	if (!shell || !ast || ft_strcmp(ast->value, "|"))
		return (1);
	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	if (fork_pipeline_children(shell, ast, pipe_fd, pids))
		return (1);
	close_pipe(pipe_fd);
	status[LEFT] = 0;
	status[RIGHT] = 0;
	init_signal_exec();
	if (wait_child_process(pids[LEFT], &status[LEFT])
		|| wait_child_process(pids[RIGHT], &status[RIGHT]))
		return (init_signal_prompt(), 1);
	init_signal_prompt();
	if (WIFEXITED(status[RIGHT]))
		return (WEXITSTATUS(status[RIGHT]));
	if (WIFSIGNALED(status[RIGHT]))
		return (report_child_signal_status(status[RIGHT]),
			128 + WTERMSIG(status[RIGHT]));
	return (1);
}
