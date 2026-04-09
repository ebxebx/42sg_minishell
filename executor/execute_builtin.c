/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/09 13:04:42 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	return (0
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "cd")
	);
}

int	run_builtin(t_shell *shell, char **argv)
{
	if (!ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	if (!ft_strcmp(argv[0], "env"))
		return (builtin_env(shell, argv));
	if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd(shell));
	if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(shell, argv));
	if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(shell, argv));
	if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit(shell, argv));
	if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd(shell, argv));
	return (1);
}

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit"));
}

int	execute_parent_builtin(t_shell *shell, char **argv)
{
	if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(shell, argv));
	if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(shell, argv));
	if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd(shell, argv));
	if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit(shell, argv));
	return (1);
}
