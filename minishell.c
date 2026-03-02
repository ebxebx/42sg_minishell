/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/02 12:51:01 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*cmd;

	(void)argc;
	(void)argv;
	init_shell(&shell, env);
	// Main loop
	while (1)
	{
		cmd = readline("minishell> ");
		if (!cmd)
			break ;
		exec_command(&shell, cmd);
		free(cmd);
	}
	free_shell(&shell);
	return (0);
}
