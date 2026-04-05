/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/05 19:43:27 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor/executor.h"
#include "minishell_prompt.h"
#include "parsing/ast.h"
#include "parsing/minishell_tokenize.h"

static char	*append_char(char *line, size_t len, char c)
{
	char	*new_line;

	new_line = malloc(len + 2);
	if (!new_line)
		return (free(line), NULL);
	if (line)
	{
		ft_memcpy(new_line, line, len);
		free(line);
	}
	new_line[len] = c;
	new_line[len + 1] = '\0';
	return (new_line);
}

static char	*read_noninteractive_line(void)
{
	char	c;
	char	*line;
	size_t	len;
	ssize_t	nread;

	line = NULL;
	len = 0;
	while (1)
	{
		nread = read(STDIN_FILENO, &c, 1);
		if (nread <= 0)
			break ;
		if (c == '\n')
			break ;
		line = append_char(line, len, c);
		if (!line)
			return (NULL);
		len++;
	}
	if (nread <= 0 && len == 0)
		return (NULL);
	if (!line)
		line = ft_strdup("");
	return (line);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*cmd;
	char	*prompt;

	init_shell(&shell, env);
	shell.debug = argc > 1 && ft_strcmp(argv[1], "debug") == 0;
	if (shell.debug)
		ft_printf("!!!Debug mode enabled\n");
	while (1)
	{
		if (shell.debug)
			prompt = build_prompt(shell.status);
		if (!shell.debug || !prompt)
			prompt = ft_strdup("minishell> ");
		init_signal_prompt();
		if (isatty(STDIN_FILENO))
			cmd = readline(prompt);
		else
			cmd = read_noninteractive_line();
		free(prompt);
		if (!cmd)
			break ;
		if (g_signal == SIGINT)
		{
			shell.status = 130;
			g_signal = 0;
		}
		if (isatty(STDIN_FILENO) && cmd[0] != '\0')
			add_history(cmd);
		exec_command(&shell, cmd);
		free(cmd);
		if (shell.should_exit)
			break ;
	}
	rl_clear_history();
	free_shell(&shell);
	close_all_fds();
	if (shell.should_exit)
		return (shell.exit_code);
	return (shell.status);
}

/*
Allowed function use in minishell:
readline, rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs
*/
