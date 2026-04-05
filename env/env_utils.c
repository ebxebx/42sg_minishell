/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/05 11:34:03 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	env_count(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

//finds length of key
int	key_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (len);
}

//matches the key only
int	match_key(char *env_entry, char *key)
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

//checks if str has '='
int	has_equal(char *str)
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

//check whether starts with _ & alpha then _ & alphanum
int	is_valid_identifier(char *str)
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

// int main(int argc, char **argv)
// {
// 	(void)argc;
// 	if(!is_valid_identifier(argv[1]))
// 	{	
// 		perror("Error");
// 		return (1);
// 	}
// 	ft_printf("Is valid\n");
// 	return (0);

// }