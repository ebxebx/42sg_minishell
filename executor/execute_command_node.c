/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/04 16:10:51 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"

static int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	return (0
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "cd")
	);
}

static int	run_builtin(t_shell *shell, char **argv)
{
	if (!ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	if (!ft_strcmp(argv[0], "env"))
		return (builtin_env(shell, argv));
	if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd(shell));
	if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(shell, argv));
	if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(shell, argv));
	if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit(shell));
	if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd(shell));
	return (1);
}

static char	*get_env_value(char **env, char *key)
{
	size_t	key_len;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

static int	exec_with_path(char **argv, char **env)
{
	char	**paths;
	char	*path_env;
	char	*tmp;
	char	*full;
	int		i;

	if (ft_strchr(argv[0], '/'))
		execve(argv[0], argv, env);
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (execve(argv[0], argv, env), 127);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (127);
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full = NULL;
		if (tmp)
			full = ft_strjoin(tmp, argv[0]);
		free(tmp);
		if (full)
			execve(full, argv, env);
		free(full);
	}
	ft_strarr_free(paths);
	return (127);
}

void	execute_command_child(t_shell *shell, t_ast *cmd)
{
	int		status;
	char	**argv;

	if (shell->debug)
		ft_printf("Executing command child: %p\n", cmd);
	if (!shell || !cmd)
		exit(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	// redirection only
	if (cmd->redirs && (!cmd->value || !cmd->value[0]))
	{
		if (apply_redirections(cmd))
			exit(1);
		exit(0);
	}
	if (!shell || !cmd || !cmd->value || !cmd->value[0])
		exit(1);
	argv = build_argv(cmd->value);
	if (!argv || !argv[0])
		exit(1);
	if (apply_redirections(cmd))
	{
		ft_strarr_free(argv);
		exit(1);
	}
	if (is_builtin_command(argv[0]))
	{
		status = run_builtin(shell, argv);
		ft_strarr_free(argv);
		exit(status);
	}
	status = exec_with_path(argv, shell->env);
	if (status == 127)
		ft_dprintf(2, "%s: command not found\n", argv[0]);
	else if (errno)
		perror(argv[0]);
	ft_strarr_free(argv);
	exit(status);
}

int	execute_command_node(t_shell *shell, t_ast *cmd)
{
	pid_t	pid;
	int		status;

	if (!shell || !cmd
		//  || !cmd->value || !cmd->value[0]
		)
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
