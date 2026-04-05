/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/05 19:30:16 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_cd(t_shell *shell, char **argv)
{
	char	*path;

	if (argv[2])
	{
		ft_putstr_fd("minishell: cd", 2);
		ft_putendl_fd(": too many arguments", 2);
		return (1);
	}
	if (!argv[1])
	{
		path = get_env_value(shell->env, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd", 2);
			ft_putendl_fd(": HOME not set", 2);
			return (1);
		}
	}
	else
		path = argv[1];
	if ((chdir(path)) == -1)
		return (perror("minishell: cd"), 1);
	return (0);
}
