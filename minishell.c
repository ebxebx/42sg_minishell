/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/02 13:49:25 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/ast.h"
#include "parsing/minishell_tokenize.h"
#include "minishell_prompt.h"

int	main(int argc, char **argv, char **env)
{
    t_shell	shell;
    char	*cmd;

    init_shell(&shell, env);
    shell.debug = argc > 1 && strcmp(argv[1], "debug") == 0;
    init_signal();
    if (shell.debug)
        fprintf(stderr, "!Debug mode enabled\n");
    // Main loop
    while (1)
    {
        char *prompt = build_prompt(shell.status);
        if (!prompt)
            prompt = strdup("minishell> ");
        cmd = readline(prompt);
        free(prompt);
        if (!cmd)
            break ;
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