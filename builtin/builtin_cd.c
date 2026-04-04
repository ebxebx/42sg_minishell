/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/04 16:18:03 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_cd(char **argv)
{
	if (!argv[1])
	{
		ft_putstr_fd("minishell: cd ", 2);
		ft_putendl_fd(": missing argument", 2);
		return (1);
	}
	if (argv[2])
	{
		ft_putstr_fd("minishell: cd ", 2);
		ft_putendl_fd(": too many arguments", 2);
		return (1);
	}
	if (chdir(argv[1]) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
