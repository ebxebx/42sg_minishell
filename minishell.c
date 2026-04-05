/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/05 13:00:59 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_prompt.h"
#include "parsing/ast.h"
#include "parsing/minishell_tokenize.h"

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*cmd;
	char	*prompt;

	init_shell(&shell, env);
	shell.debug = argc > 1 && ft_strcmp(argv[1], "debug") == 0;
	if (shell.debug)
		ft_printf("!!!Debug mode enabled\n");
	// Main loop
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
		if (g_signal == SIGINT)
		{
			shell.status = 130;
			g_signal = 0;
		}
		if (cmd[0] != '\0')
			add_history(cmd);
		exec_command(&shell, cmd);
		free(cmd);
	}
	free_shell(&shell);
	return (0);
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
