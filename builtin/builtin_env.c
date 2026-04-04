/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/04 21:18:15 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_env(t_shell *shell, char **argv)
{
	int	i;

	if (argv[1])
	{
		ft_putendl_fd("minishell: env: too many arguments", 2);
		return (1);
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
