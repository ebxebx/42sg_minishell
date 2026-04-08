/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:35:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 18:35:47 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
// # include <stdio.h>
// # include <stdlib.h>
// # include <string.h>
// # include <sys/types.h>
# include <sys/wait.h>
// # include <unistd.h>
# include "parsing/ast.h"
# include "parsing/minishell_tokenize.h"

typedef struct s_shell
{
	int				debug;
	int				status;
	int				should_exit;
	int				exit_code;
	char			**env;
	char			**export;
	t_token			*tokens;
	t_ast			*ast;
}					t_shell;

extern sig_atomic_t	g_signal;

void				init_shell(t_shell *shell, char **env);
int					exec_command(t_shell *shell, char *cmd);
void				free_shell(t_shell *shell);

void				init_signal_prompt(void);
void				init_signal_exec(void);
void				init_signal_heredoc(void);

void				sigint_prompt_handler(int signo);
void				sigint_exec_handler(int signo);
void				sigint_heredoc_handler(int signo);

#endif
