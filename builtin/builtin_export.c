/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/03/30 18:09:48 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int		builtin_export(t_shell *shell, char **argv);
int		export_arg(t_shell *shell, char *str);
int		mark_export(char ***env, char *key);
int		set_env_var(char ***env, char *str);

int	builtin_export(t_shell *shell, char **argv)
{
	int	i;

	if (argv[0])
	{
		while (shell->export && shell->export[i])
		{
			if (ft_strchr(shell->env[i], '='))
				ft_putendl_fd(shell->env[i], 1);
			else
			{
				ft_putendl_fd(shell->env[i], 1);
				ft_putstr_fd("=' '", 1);
			}
			i++;
		}
	}
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			perror("export");
			return (1);
		}
		if (has_equal(argv[i]))
			assign_export(&shell, argv[i]);
		else
			mark_exported(&shell, argv[i]);
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