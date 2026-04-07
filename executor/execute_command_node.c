/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/07 21:22:01 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"
#include "executor.h"
#include <sys/stat.h>
#include <sys/types.h>

static void	exit_command_child(t_shell *shell, int status)
{
	rl_clear_history();
	if (shell)
		free_shell(shell);
	close_all_fds();
	exit(status);
}

static void	redir_only_or_empty_command_child(t_shell *shell, t_ast *cmd)
{
	if (cmd->redirs && (!cmd->value || !cmd->value[0]))
	{
		if (apply_redirections(cmd))
			exit_command_child(shell, 1);
		exit_command_child(shell, 0);
	}
	if (!cmd->value[0])
	{
		if (!cmd->argv || !cmd->argv[0])
			exit_command_child(shell, 0);
		ft_dprintf(2, ": command not found\n");
		exit_command_child(shell, 127);
	}
	if (!shell || !cmd || !cmd->value)
		exit_command_child(shell, 1);
}

void	execute_command_child(t_shell *shell, t_ast *cmd)
{
	int		status;
	char	**argv;

	if (shell->debug)
		ft_printf("Executing command child: %p, value: %s\n", cmd, cmd->value);
	if (!shell || !cmd)
		exit(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	redir_only_or_empty_command_child(shell, cmd);
	argv = cmd->argv;
	if (!argv || apply_redirections(cmd))
		exit_command_child(shell, 1);
	if (is_builtin_command(argv[0]))
	{
		status = run_builtin(shell, argv);
		exit_command_child(shell, status);
	}
	status = exec_with_path(argv, shell->env);
	if (status == 127 && !ft_strchr(argv[0], '/'))
		ft_dprintf(2, "%s: command not found\n", argv[0]);
	else if (status != 0 && errno)
		perror(argv[0]);
	exit_command_child(shell, status);
}

int	execute_command_node(t_shell *shell, t_ast *cmd)
{
	pid_t	pid;
	int		status;

	if (!shell || !cmd)
		return (1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		execute_command_child(shell, cmd);
	init_signal_exec();
	while (waitpid(pid, &status, 0) < 0)
	{
		if (errno != EINTR)
			return (init_signal_prompt(), perror("waitpid"), 1);
	}
	init_signal_prompt();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
