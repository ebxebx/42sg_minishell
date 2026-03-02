/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/02 13:18:00 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->status = 0;
	shell->env = env;
}

void	free_shell(t_shell *shell)
{
	free(shell->env);
}

void	exec_command(t_shell *shell, char *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		char **args = ft_split(cmd, ' ');
		if (!args)
			exit(EXIT_FAILURE);
		execve(args[0], args, shell->env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		return ;
	}
	// Parent process
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->status = WEXITSTATUS(status);
}
