/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/05 12:47:17 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor/executor.h"
#include "parsing/ast.h"
#include "parsing/minishell_tokenize.h"
#include "expander/expand.h"


void	exec_command(t_shell *shell, char *cmd)
{
	shell->tokens = parse_token(cmd);
	if (!shell->tokens)
		return ;
	if (!expand_tokens(shell->tokens, shell))
	{
		free_token_list(shell->tokens);
		shell->status = 1;
		return ;
	}
	shell->ast = parse_tokens_to_ast(shell->tokens);
	if (!shell->ast)
	{
		free_token_list(shell->tokens);
		shell->status = 1;
		return ;
	}
	if (shell && shell->debug && shell->ast)
		print_ast(shell->ast, 0);
	shell->status = execute_ast(shell, shell->ast);
	free_ast(shell->ast);
	shell->ast = NULL;
	free_token_list(shell->tokens);
	shell->tokens = NULL;
}
