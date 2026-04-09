/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/09 13:02:01 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	sort_export(char **arg)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (arg[i])
	{
		j = 0;
		while (arg[j] && arg[j + 1])
		{
			if ((ft_strcmp(arg[j], arg[j + 1])) > 0)
			{
				temp = arg[j];
				arg[j] = arg[j + 1];
				arg[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
