/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/09 15:27:50 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static char	*get_key(char *arg)
{
	char	*key;

	key = ft_strchr(arg, '=');
	if (!key)
		return (ft_strdup(arg));
	return (ft_substr(arg, 0, key - arg));
}

static void	swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_export(char **arg)
{
	int		i;
	int		j;
	char	*key1;
	char	*key2;

	i = 0;
	while (arg[i])
	{
		j = 0;
		while (arg[j] && arg[j + 1])
		{
			key1 = get_key(arg[j]);
			key2 = get_key(arg[j + 1]);
			if ((key1 && key2) && (ft_strcmp(key1, key2) > 0))
				swap(&arg[j], &arg[j + 1]);
			if (key1)
				free(key1);
			if (key2)
				free(key2);
			j++;
		}
		i++;
	}
}
