/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify_env+export.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/03/31 12:08:47 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	env_index(char **env, char *key)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (match_key(env[i], key))
			return (i);
		i++;
	}
	return (-1);
}

//for "export a, checks if variable alrd exists. y, return, n, add to export"
int	mark_exported(t_shell *shell, char *str)
{
	char	**new_export;

	if (env_index(shell->export, str) != -1)
		return (0);
	new_export = add_env_entry(shell->export, str);
	if (!new_export)
		return (1);
	shell->export = new_export;
	return (0);
}

//for "export a= and a=42"
int	assign_export(t_shell *shell, char *str)
{
	char	**new_export;
	char	**new_env;

	new_env = update_or_add(shell->env, str);
	if (!new_env)
		return (1);
	shell->env = new_env;
	new_export = update_or_add(shell->export, str);
	if (!new_export)
		return (1);
	shell->export = new_export;
	return (0);
}

char	**dup_env(char **env)
{
	int		i;
	int		n;
	char	**copy;

	n = env_count(env);
	copy = malloc(sizeof(char *) * (n + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < n)
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			copy[i] = NULL;
			ft_strarr_free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
