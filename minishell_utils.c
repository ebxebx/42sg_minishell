/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/28 22:59:20 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int env_count(char **env)
{
	int	i;
	while (env && env[i])
		i++;
	return (i);
}

static char	**dup_env(char **env)
{
	int		i;
	int 	n;
	char	**copy;
	
	n = env_count(env);
	copy = malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = ft_strdup(env[i]);
		if(!copy[i])
		{
			while (i >= 0)
			{
				i--;
				free(copy[i]);
			}
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[n] = NULL;
	return (copy);
}

void init_shell(t_shell *shell, char **env)
{
	shell->status = 0;
	shell->env = dup_env(env);
	if (!shell->env)
		exit(EXIT_FAILURE);
}

void free_shell(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env && shell->env[i])
	{
		free(shell->env[i]);
		i++;
	}
	free(shell->env);
}

void exec_command(t_shell *shell, char *cmd)
{
	pid_t pid;
	int status;

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
		return;
	}
	// Parent process
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->status = WEXITSTATUS(status);
}
