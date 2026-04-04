/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modify_entry.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/04 22:29:16 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

//add new key=value entry and free old env array
char	**add_env_entry(char **env, char *new_entry)
{
	int		i;
	int		n;
	char	**new_env;

	n = env_count(env);
	new_env = malloc(sizeof(char *) * (n + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < n)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = ft_strdup(new_entry);
	if (!new_env[i])
	{
		free(new_env);
		return (NULL);
	}
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}

//assign copy to new entry, find line index, replace w copy, then free 
char	**replace_env_entry(char **env, int index, char *new_entry)
{
	char	*copy;

	copy = ft_strdup(new_entry);
	if (!copy)
		return (NULL);
	free(env[index]);
	env[index] = copy;
	return (env);
}

//either replace or add
char	**update_or_add(char **env, char *str)
{
	int	index;

	index = env_index(env, str);
	if (index != -1)
		return (replace_env_entry(env, index, str));
	return (add_env_entry(env, str));
}

//find index, free line, assign to next line, then last point to null
char	**remove_env_entry(char **env, char *str)
{
	int	index;

	index = env_index(env, str);
	if (index == -1)
		return (env);
	free(env[index]);
	while (env[index + 1])
	{
		env[index] = env[index + 1];
		index++;
	}
	env[index] = NULL;
	return (env);
}
