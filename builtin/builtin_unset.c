/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/07 16:21:24 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/* Bash treats leading `-` names as invalid unset options, not variables. */
static int	unset_option_error(char *arg)
{
	ft_putstr_fd("minishell: unset: ", 2);
	ft_putchar_fd(arg[0], 2);
	ft_putchar_fd(arg[1], 2);
	ft_putendl_fd(": invalid option", 2);
	ft_putendl_fd("unset: usage: unset [-f] [-v] [-n] [name ...]", 2);
	return (2);
}

int	builtin_unset(t_shell *shell, char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
			return (unset_option_error(argv[i]));
		if (is_valid_identifier(argv[i]))
		{
			shell->env = remove_env_entry(shell->env, argv[i]);
			shell->export = remove_env_entry(shell->export, argv[i]);
		}
		i++;
	}
	return (0);
}
