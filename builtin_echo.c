/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:34 by ka-tan            #+#    #+#             */
/*   Updated: 2026/03/23 18:39:54 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_flag(char *s)
{
	int	i;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	i = 1;
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	if (!argv)
		return (1);
	i = 1;
	newline = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		write (1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write (1, " ", 1);
		i++;
	}
	if (newline)
		write (1, "\n", 1);
	return (0);
}

//int main(int argc, char **argv)
//{
//	(void)argc;
//	builtin_echo(argv);
//}