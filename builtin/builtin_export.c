/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/07 16:22:18 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	is_underscore_var(char *entry)
{
	if (!entry)
		return (0);
	return (entry[0] == '_' && (entry[1] == '=' || entry[1] == '\0'));
}

static int	print_export_option_error(char *arg)
{
	if (!ft_strncmp(arg, "--", 2) && arg[2] != '\0')
		ft_putendl_fd("minishell: export: --: invalid option", 2);
	else
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putchar_fd(arg[0], 2);
		ft_putchar_fd(arg[1], 2);
		ft_putendl_fd(": invalid option", 2);
	}
	ft_putendl_fd("export: usage: export [-fn] [name[=value] ...]"
		" or export -p", 2);
	return (2);
}

void	print_export_entry(char *entry)
{
	int	i;
	int	len;

	if (!entry)
		return ;
	len = key_len(entry);
	ft_putstr_fd("declare -x ", 1);
	i = 0;
	while (i < len)
	{
		ft_putchar_fd(entry[i], 1);
		i++;
	}
	if (has_equal(entry))
	{
		ft_putstr_fd("=\"", 1);
		i = len + 1;
		while (entry[i])
		{
			ft_putchar_fd(entry[i], 1);
			i++;
		}
		ft_putstr_fd("\"", 1);
	}
	ft_putchar_fd('\n', 1);
}

int	print_export(char **export)
{
	int	i;

	i = 0;
	while (export && export[i])
	{
		if (!is_underscore_var(export[i]))
			print_export_entry(export[i]);
		i++;
	}
	return (0);
}

int	builtin_export(t_shell *shell, char **argv)
{
	int	i;
	int	status;

	if (!argv[1])
		return (print_export(shell->export));
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (argv[i][0] == '-' && argv[i][1] != '\0')
			return (print_export_option_error(argv[i]));
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putendl_fd(": not a valid identifier", 2);
			status = 1;
		}
		else if (has_equal(argv[i]))
			assign_export(shell, argv[i]);
		else
			mark_exported(shell, argv[i]);
		i++;
	}
	return (status);
}

/* int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	int		status;

	init_shell(&shell, env);
	status = 0;
	if (argc < 2)
		status = 1;
	else if (!ft_strcmp(argv[1], "env"))
		status = builtin_env(&shell, argv + 1);
	else if (!ft_strcmp(argv[1], "export"))
		status = builtin_export(&shell, argv + 1);
	free_shell(&shell);
	return (status);
} */
