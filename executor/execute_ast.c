/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/30 00:00:00 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	execute_ast(t_shell *shell, t_ast *ast)
{
	if (!shell || !ast)
		return (1);
	if (!ft_strcmp(ast->value, "|"))
		return (execute_pipeline(shell, ast));
	return (execute_command_node(shell, ast));
}

void	execute_ast_child(t_shell *shell, t_ast *ast)
{
	if (!shell || !ast)
		exit(1);
	if (!ft_strcmp(ast->value, "|"))
		exit(execute_pipeline(shell, ast));
	execute_command_child(shell, ast);
}
