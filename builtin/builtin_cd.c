/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/09 15:03:17 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "builtin_cd.h"

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
	if (argv[1] && argv[2])
	{
		ft_putstr_fd("minishell: cd", 2);
		ft_putendl_fd(": too many arguments", 2);
		return (1);
	}
	return (0);
}

static int	resolve_cd_target(t_shell *shell, char **argv,
	t_path_info *path_info)
{
	if (!argv[1] || !ft_strcmp(argv[1], "--") || !ft_strcmp(argv[1], "~"))
	{
		path_info->path = get_env_value(shell->env, "HOME");
		if (!(path_info->path))
		{
			ft_putstr_fd("minishell: cd", 2);
			ft_putendl_fd(": HOME not set", 2);
			return (1);
		}
	}
	else if (!ft_strcmp(argv[1], "-"))
	{
		path_info->path = get_env_value(shell->env, "OLDPWD");
		if (!(path_info->path) || !(path_info->path)[0])
		{
			ft_putstr_fd("minishell: cd", 2);
			ft_putendl_fd(": OLDPWD not set", 2);
			return (1);
		}
		path_info->print_path = 1;
		path_info->path_copy = ft_strdup(path_info->path);
		if (!(path_info->path_copy))
			return (1);
	}
	return (0);
}

/* `cd ~` uses HOME. */
/* `cd --` uses HOME; `--` only stops option parsing here. */
/* `cd -` switches to OLDPWD and prints the destination path. */
int	builtin_cd(t_shell *shell, char **argv)
{
	t_path_info	path_info;

	path_info.path = NULL;
	path_info.path_copy = NULL;
	path_info.print_path = 0;
	if (check_argv_count(shell, argv))
		return (1);
	if (resolve_cd_target(shell, argv, &path_info))
		return (1);
	if (!path_info.path)
		path_info.path = argv[1];
	if ((chdir(path_info.path)) == -1)
		return (free(path_info.path_copy), perror("minishell: cd"), 1);
	if (update_pwd_vars(shell))
		return (free(path_info.path_copy), 1);
	if (path_info.print_path)
		ft_putendl_fd(path_info.path_copy, 1);
	free(path_info.path_copy);
	return (0);
}
