/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/03/30 02:30:43 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "env.h"
#include "../libft/libft.h"

int	env_count(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

int	key_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (len);
}
int		match_key(char *env_entry, char *key)
{
	int	len;

	if (!env_entry || !key)
		return (0);
	len = key_len(key);
	if (ft_strncmp(env_entry, key, len) == 0)
	{
		if (env_entry[len] == '=' || env_entry[len] == '\0')
			return (1);
	}
	return (0);
}
{}
int		env_index(char **env, char *key)
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

int		has_equal(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int		is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
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

// int main(int argc, char **argv)
// {
// 	(void)argc;
// 	if(!is_valid_identifier(argv[1]))
// 	{	
// 		perror("Error");
// 		return (1);
// 	}
// 	printf("Is valid\n");
// 	return (0);

// }