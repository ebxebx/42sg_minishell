/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:26 by ka-tan            #+#    #+#             */
/*   Updated: 2026/03/30 18:11:06 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	is_builtin(char *builtin_cmd)
{
	if (!builtin_cmd)
		return (0);
	else if (!ft_strcmp(builtin_cmd, "echo"))
		return (1);
	else if (!ft_strcmp(builtin_cmd, "env"))
		return (1);
	else if (!ft_strcmp(builtin_cmd, "pwd"))
		return (1);
	else if (!ft_strcmp(builtin_cmd, "cd"))
		return (1);
	else if (!ft_strcmp(builtin_cmd, "export"))
		return (1);
	else if (!ft_strcmp(builtin_cmd, "unset"))
		return (1);
	else if (!ft_strcmp(builtin_cmd, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(t_shell *shell, char **argv)
{
	if (!argv || !argv[0])
		return (0);
	else if (!ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	else if (!ft_strcmp(argv[0], "env"))
		return (builtin_env(shell, argv));
	else if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd());
	else if (!ft_strcmp(argv[0], "export"))
		return (builtin_export());
	else if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset());
	else if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit());
	return (0);
}
