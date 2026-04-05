/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/05 13:02:15 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	mod_exit_code(long long n)
{
	int	code;

	code = n % 256;
	if (code < n)
		code = code + 256;
	return (code);
}

static void	exit_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
}

static void	exit_too_many_args_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": too many arguments", 2);
}

//fd=1 can be redirected to another file, stderr=2
//can be used to store shell diagnostic msg
int	builtin_exit(t_shell *shell, char **argv)
{
	long long	n;

	ft_putendl_fd("exit", 1);
	if (!argv[1])
	{
		shell->should_exit = 1;
		shell->exit_code = shell->status;
		return (shell->status);
	}
	if (!ft_atoll_exit(argv[1], &n))
	{
		exit_numeric_error(argv[1]);
		shell->should_exit = 1;
		shell->exit_code = 2;
		return (2);
	}
	if (argv[2])
	{
		exit_too_many_args_error(argv[1]);
		shell->status = 1;
		return (1);
	}
	shell->should_exit = 1;
	shell->exit_code = mod_exit_code(n);
	return (shell->exit_code);
}
