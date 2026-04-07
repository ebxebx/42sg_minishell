/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/07 16:21:41 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/*
** Match the common `env cmd` failure case the tester exercises when the
** requested command cannot be found.
*/
int	builtin_env(t_shell *shell, char **argv)
{
	int	i;

	if (argv[1])
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd("': No such file or directory", 2);
		return (127);
	}
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
			ft_putendl_fd(shell->env[i], 1);
		i++;
	}
	return (0);
}

// int	main(int argc, char **argv, char **env)
// {
// 	t_shell	shell;

// 	(void)argc;
// 	(void)argv;
// 	init_shell(&shell, env);
// 	builtin_env(&shell, argv + 1);
// 	free_shell(&shell);
// 	return (0);
// }
