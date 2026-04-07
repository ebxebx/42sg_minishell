/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/07 16:39:07 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	update_pwd_vars(t_shell *shell)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*entry;

	old_pwd = get_env_value(shell->env, "PWD");
	if (old_pwd && old_pwd[0])
	{
		entry = ft_strjoin("OLDPWD=", old_pwd);
		if (!entry)
			return (1);
		assign_export(shell, entry);
		free(entry);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (1);
	entry = ft_strjoin("PWD=", new_pwd);
	free(new_pwd);
	if (!entry)
		return (1);
	assign_export(shell, entry);
	free(entry);
	return (0);
}

static int	check_argv_count(t_shell *shell, char **argv)
{
	(void)shell;
	if (argv[2])
	{
		ft_putstr_fd("minishell: cd", 2);
		ft_putendl_fd(": too many arguments", 2);
		return (1);
	}
	return (0);
}

static int	check_dash_argv(t_shell *shell, char **argv)
{
	char	*home;
	char	*oldpwd;

	if (!ft_strcmp(argv[1], "--"))
	{
		home = get_env_value(shell->env, "HOME");
		if (!home)
		{
			ft_putstr_fd("minishell: cd", 2);
			ft_putendl_fd(": HOME not set", 2);
			return (1);
		}
	}
	else if (!ft_strcmp(argv[1], "-"))
	{
		oldpwd = get_env_value(shell->env, "OLDPWD");
		if (!oldpwd || !oldpwd[0])
		{
			ft_putstr_fd("minishell: cd", 2);
			ft_putendl_fd(": OLDPWD not set", 2);
			return (1);
		}
	}
	return (0);
}

/* `cd --` uses HOME; `--` only stops option parsing here. */
/* `cd -` switches to OLDPWD and prints the destination path. */
int	builtin_cd(t_shell *shell, char **argv)
{
	char	*path;
	char	*path_copy;
	int		print_path;

	path_copy = NULL;
	print_path = 0;
	if (check_argv_count(shell, argv))
		return (1);
	if (check_dash_argv(shell, argv))
		return (1);
	else
		path = argv[1];
	if ((chdir(path)) == -1)
		return (free(path_copy), perror("minishell: cd"), 1);
	if (update_pwd_vars(shell))
		return (free(path_copy), 1);
	if (print_path)
		ft_putendl_fd(path_copy, 1);
	free(path_copy);
	return (0);
}
