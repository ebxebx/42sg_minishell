/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/06 15:14:47 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env/env.h"
#include "libft/libft.h"
#include "minishell.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->debug = 0;
	shell->ast = NULL;
	shell->tokens = NULL;
	shell->status = 0;
	shell->should_exit = 0;
	shell->exit_code = 0;
	shell->env = dup_env(env);
	shell->export = dup_env(env);
	if (!shell->env || !shell->export)
		exit(EXIT_FAILURE);
}

void	free_shell(t_shell *shell)
{
	ft_strarr_free(shell->env);
	ft_strarr_free(shell->export);
	if (shell->ast)
		free_ast(shell->ast);
	if (shell->tokens)
		free_token_list(shell->tokens);
}
