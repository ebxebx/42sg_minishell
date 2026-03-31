/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/03/31 13:43:27 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_unset(t_shell *shell, char **argv);
int	unset_arg(char ***env, char *arg);

int	builtin_unset(t_shell *shell, char **argv)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putendl_fd(": not a valid identifier", 2);
			status = 1;
		}
		else
		{
			shell->env = remove_env_entry(shell->env, argv[i]);
			shell->export = remove_env_entry(shell->export, argv[i]);
		} 
		i++;
	}
	return (status);
}