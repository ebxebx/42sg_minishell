/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/04 00:36:00 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor/executor.h"
#include "parsing/ast.h"
#include "parsing/minishell_tokenize.h"
#include "expander/expand.h"


void	exec_command(t_shell *shell, char *cmd)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = parse_token(cmd);
	if (!tokens)
		return ;
	if (!expand_tokens(tokens, shell))
	{
		free_token_list(tokens);
		shell->status = 1;
		return ;
	}
	ast = parse_tokens_to_ast(tokens);
	if (!ast)
	{
		free_token_list(tokens);
		shell->status = 1;
		return ;
	}
	if (shell && shell->debug && ast)
		print_ast(ast, 0);
	shell->status = execute_ast(shell, ast);
	free_ast(ast);
	free_token_list(tokens);
}
