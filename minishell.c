/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/07 12:21:19 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor/executor.h"
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

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*cmd;
	char	*prompt;

	init_shell(&shell, env);
	init_debug_mode(&shell, argc, argv);
	while (1)
	{
		if (shell.debug)
			prompt = build_prompt(shell.status);
		if (!shell.debug || !prompt)
			prompt = ft_strdup("minishell> ");
		init_signal_prompt();
		cmd = readline(prompt);
		free(prompt);
		if (!cmd)
			break ;
		handle_signal_status(&shell);
		if (cmd[0] != '\0')
			add_history(cmd);
		exec_command(&shell, cmd);
		free(cmd);
	}
	rl_clear_history();
	return (close_all_fds(), free_shell(&shell), 0);
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
