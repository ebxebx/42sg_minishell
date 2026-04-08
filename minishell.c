/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 18:48:29 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"
#include "minishell.h"
#include "minishell_prompt.h"
#include "parsing/ast.h"
#include "parsing/minishell_tokenize.h"

static void	init_debug_mode(t_shell *shell, int argc, char **argv)
{
	shell->debug = 0;
	if (argc > 1 && ft_strcmp(argv[1], "debug") == 0)
		shell->debug = 1;
	if (shell->debug)
		ft_printf("!!!Debug mode enabled\n");
}

static void	handle_signal_status(t_shell *shell)
{
	if (g_signal == SIGINT)
	{
		shell->status = 130;
		g_signal = 0;
	}
}

static char	*prompt(t_shell *shell)
{
	char	*prompt;
	char	*cmd;

	if (shell->debug)
		prompt = build_prompt(shell->status);
	if (!shell->debug || !prompt)
		prompt = ft_strdup("minishell> ");
	init_signal_prompt();
	if (isatty(STDIN_FILENO))
		cmd = readline(prompt);
	else
		cmd = read_noninteractive_line();
	free(prompt);
	return (cmd);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*cmd;

	init_shell(&shell, env);
	init_debug_mode(&shell, argc, argv);
	while (1)
	{
		cmd = prompt(&shell);
		if (!cmd)
			break ;
		handle_signal_status(&shell);
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
