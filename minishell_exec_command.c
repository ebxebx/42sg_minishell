/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 18:36:33 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"
#include "expander/expand.h"
#include "minishell.h"
#include "parsing/ast.h"
#include "parsing/minishell_tokenize.h"

static int	is_blank_command(char *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return (1);
	while (cmd[i])
	{
		if (ft_isspace(cmd[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
	return value not used,
		just want to make use of return(xxx,xxxx) to shorten line
*/
int	exec_command(t_shell *shell, char *cmd)
{
	shell->tokens = parse_token(cmd);
	if (!shell->tokens)
	{
		if (!is_blank_command(cmd))
			shell->status = 2;
		return (shell->status);
	}
	if (!expand_tokens(shell->tokens, shell))
		return (free_token_list(shell->tokens), shell->tokens = NULL,
			shell->status = 1, shell->status);
	shell->ast = parse_tokens_to_ast(shell->tokens);
	if (!shell->ast)
		return (free_token_list(shell->tokens), shell->tokens = NULL,
			shell->status = 2, shell->status);
	if (shell && shell->debug && shell->ast)
		print_ast(shell->ast, 0);
	shell->status = execute_ast(shell, shell->ast);
	free_ast(shell->ast);
	shell->ast = NULL;
	free_token_list(shell->tokens);
	shell->tokens = NULL;
	return (shell->status);
}
