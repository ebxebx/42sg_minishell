/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:40 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/04 23:12:35 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

//checks if path1 & 2 is same directory by comparing st_dev and st_ino
static int	same_directory(const char *path1, const char *path2)
{
	struct stat	st1;
	struct stat	st2;

	if (stat(path1, &st1) != 0)
		return (0);
	if (stat(path2, &st2) != 0)
		return (0);
	return (st1.st_dev == st2.st_dev && st1.st_ino == st2.st_ino);
}

//if PWD is valid, return copy of PWD, else return getcwd
char	*logical_getcwd(char **env)
{
	char	*pwd;
	char	*cwd;

	pwd = get_env_value(env, "PWD");
	if (pwd && pwd[0] == '/' && same_directory(".", pwd))
		return (ft_strdup(pwd));
	cwd = getcwd(NULL, 0);
	return (cwd);
}

int	builtin_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = logical_getcwd(shell->env);
	if (cwd != NULL)
		ft_putendl_fd(cwd, 1);
	else
	{
		perror("pwd");
		return (1);
	}
	free(cwd);
	return (0);
}

/*
int	main(void)
{
	t_shell	shell;
	int		status;

	status = builtin_pwd(&shell);
	ft_printf("return value = %d\n", status);
	return (0);
} */
