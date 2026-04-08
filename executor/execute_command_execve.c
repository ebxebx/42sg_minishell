/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_execve.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:24:40 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin/builtin.h"
#include "../parsing/tokenize.h"
#include "executor.h"
#include <sys/stat.h>
#include <sys/types.h>

static int	exit_exec(char **paths, int saw_eacces)
{
	ft_strarr_free(paths);
	if (saw_eacces)
		return (errno = EACCES, 126);
	errno = ENOENT;
	return (127);
}

static int	exec_sh_fallback(char *script, char **argv, char **env)
{
	size_t	i;
	size_t	argc;
	char	**sh_argv;

	argc = 0;
	while (argv[argc])
		argc++;
	sh_argv = malloc(sizeof(char *) * (argc + 2));
	if (!sh_argv)
		return (126);
	sh_argv[0] = "/bin/sh";
	sh_argv[1] = script;
	i = 1;
	while (i < argc)
	{
		sh_argv[i + 1] = argv[i];
		i++;
	}
	sh_argv[argc + 1] = NULL;
	execve("/bin/sh", sh_argv, env);
	free(sh_argv);
	if (errno == ENOENT)
		return (127);
	return (126);
}

static int	exec_full_path(char **argv, char **env)
{
	struct stat	st;

	if (stat(argv[0], &st) == -1)
	{
		if (errno == ENOENT)
			return (127);
		return (126);
	}
	if (S_ISDIR(st.st_mode))
	{
		errno = EISDIR;
		return (126);
	}
	execve(argv[0], argv, env);
	if (errno == ENOEXEC)
		return (exec_sh_fallback(argv[0], argv, env));
	if (errno == ENOENT)
		return (127);
	return (126);
}

static int	exec_find_path(char **argv, char **env, char **paths)
{
	char	*full;
	int		i;
	int		saw_eacces;

	i = -1;
	saw_eacces = 0;
	while (paths[++i])
	{
		full = get_full_path(argv[0], paths[i]);
		if (full)
		{
			execve(full, argv, env);
			if (errno == ENOEXEC)
			{
				i = exec_sh_fallback(full, argv, env);
				free(full);
				ft_strarr_free(paths);
				return (i);
			}
			if (errno == EACCES)
				saw_eacces = 1;
		}
		free(full);
	}
	return (exit_exec(paths, saw_eacces));
}

int	exec_with_path(char **argv, char **env)
{
	char	**paths;

	if (ft_strchr(argv[0], '/') != NULL)
		return (exec_full_path(argv, env));
	paths = get_paths_from_env(env);
	if (!paths)
		return (127);
	return (exec_find_path(argv, env, paths));
}
