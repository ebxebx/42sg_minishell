/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/05 19:30:16 by ka-tan           ###   ########.fr       */
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

int	builtin_cd(t_shell *shell, char **argv)
{
	char	*path;
	char	*path_copy;
	int		print_path;

	path_copy = NULL;
	print_path = 0;
	if (argv[2])
	{
		ft_putstr_fd("minishell: cd", 2);
		ft_putendl_fd(": too many arguments", 2);
		return (1);
	}
	if (!argv[1] || !ft_strcmp(argv[1], "--"))
	{
		/* `cd --` uses HOME; `--` only stops option parsing here. */
		path = get_env_value(shell->env, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd", 2);
			ft_putendl_fd(": HOME not set", 2);
			return (1);
		}
	}
	else if (!ft_strcmp(argv[1], "-"))
	{
		/* `cd -` switches to OLDPWD and prints the destination path. */
		path = get_env_value(shell->env, "OLDPWD");
		if (!path || !path[0])
		{
			ft_putstr_fd("minishell: cd", 2);
			ft_putendl_fd(": OLDPWD not set", 2);
			return (1);
		}
		print_path = 1;
		path_copy = ft_strdup(path);
		if (!path_copy)
			return (1);
	}
	else
		path = argv[1];
	if ((chdir(path)) == -1)
		return (free(path_copy), perror("minishell: cd"), 1);
	/* Keep logical shell state aligned with the directory change. */
	if (update_pwd_vars(shell))
		return (free(path_copy), 1);
	if (print_path)
		ft_putendl_fd(path_copy, 1);
	free(path_copy);
	return (0);
}
