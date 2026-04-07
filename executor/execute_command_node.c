/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/07 18:37:52 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"

#include <sys/types.h>
#include <sys/stat.h>

static void	exit_command_child(t_shell *shell, int status)
{
	rl_clear_history();
	if (shell)
		free_shell(shell);
	close_all_fds();
	exit(status);
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

static int	exec_with_path(char **argv, char **env)
{
	char		**paths;
	char		*path_env;
	char		*tmp;
	char		*full;
	int			i;
	int			has_slash;
	int			saw_eacces;
	struct stat	st;

	has_slash = (ft_strchr(argv[0], '/') != NULL);
	if (has_slash)
	{
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
	path_env = get_env_value(env, "PATH");
	if (path_env && !*path_env)
	{
		errno = ENOENT;
		return (127);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
		return (127);
	i = -1;
	saw_eacces = 0;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full = NULL;
		if (tmp)
			full = ft_strjoin(tmp, argv[0]);
		free(tmp);
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
	ft_strarr_free(paths);
	if (saw_eacces)
		return (errno = EACCES, 126);
	errno = ENOENT;
	return (127);
}

void	execute_command_child(t_shell *shell, t_ast *cmd)
{
	int		status;
	char	**argv;

	if (shell->debug)
		ft_printf("Executing command child: %p, value: %s\n", cmd, cmd->value);
	if (!shell || !cmd)
		exit(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->redirs && (!cmd->value || !cmd->value[0]))
	{
		if (apply_redirections(cmd))
			exit_command_child(shell, 1);
		exit_command_child(shell, 0);
	}
	if (!cmd->value[0])
	{
		if (!cmd->argv || !cmd->argv[0])
			exit_command_child(shell, 0);
		ft_dprintf(2, ": command not found\n");
		exit_command_child(shell, 127);
	}
	if (!shell || !cmd || !cmd->value)
		exit_command_child(shell, 1);
	argv = cmd->argv;
	if (!argv)
		exit_command_child(shell, 1);
	if (apply_redirections(cmd))
		exit_command_child(shell, 1);
	if (is_builtin_command(argv[0]))
	{
		status = run_builtin(shell, argv);
		exit_command_child(shell, status);
	}
	status = exec_with_path(argv, shell->env);
	if (status == 127 && !ft_strchr(argv[0], '/'))
		ft_dprintf(2, "%s: command not found\n", argv[0]);
	else if (status != 0 && errno)
		perror(argv[0]);
	exit_command_child(shell, status);
}

int	execute_command_node(t_shell *shell, t_ast *cmd)
{
	pid_t	pid;
	int		status;

	if (!shell || !cmd)
		return (1);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		execute_command_child(shell, cmd);
	init_signal_exec();
	while (waitpid(pid, &status, 0) < 0)
	{
		if (errno != EINTR)
			return (init_signal_prompt(), perror("waitpid"), 1);
	}
	init_signal_prompt();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
