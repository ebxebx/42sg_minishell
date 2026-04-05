/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/05 12:59:32 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env/env.h"
#include "libft/libft.h"
#include "minishell.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->debug = 0;
	shell->status = 0;
	shell->env = dup_env(env);
	shell->export = dup_env(env);
	if (!shell->env || !shell->export)
		exit(EXIT_FAILURE);
}

void	free_shell(t_shell *shell, char **argv)
{
	ft_strarr_free(shell->env);
	ft_strarr_free(shell->export);
	if (shell->ast)
		free_ast(shell->ast);
	if (shell->tokens)
		free_token_list(shell->tokens);
	if (argv)
		ft_strarr_free(argv);
}
