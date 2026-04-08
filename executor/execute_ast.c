/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:24:40 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../builtin/builtin.h"
#include "../parsing/tokenize.h"

static int	execute_parent_builtin_node(t_shell *shell, t_ast *ast, char **argv)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
		return (perror("dup"), 1);
	if (apply_redirections(ast))
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	status = execute_parent_builtin(shell, argv);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}

static void	exit_shell_parent(t_shell *shell)
{
	int	exit_code;

	exit_code = shell->exit_code;
	rl_clear_history();
	free_shell(shell);
	close_all_fds();
	exit(exit_code);
}

static void	exit_ast_child(t_shell *shell, int status)
{
	if (shell)
		free_shell(shell);
	close_all_fds();
	exit(status);
}

int	execute_ast(t_shell *shell, t_ast *ast)
{
	char	**argv;
	int		status;

	if (!shell || !ast)
		return (1);
	if (preprocess_heredocs(ast, shell))
	{
		if (g_signal == SIGINT)
			return (g_signal = 0, 130);
		return (1);
	}
	if (!ft_strcmp(ast->value, "|"))
		return (execute_pipeline(shell, ast));
	argv = ast->argv;
	if (argv && argv[0] && is_parent_builtin(argv[0]))
	{
		status = execute_parent_builtin_node(shell, ast, argv);
		if (shell->should_exit)
			exit_shell_parent(shell);
		return (status);
	}
	return (execute_command_node(shell, ast));
}

void	execute_ast_child(t_shell *shell, t_ast *ast)
{
	if (!shell || !ast)
		exit(1);
	if (!ft_strcmp(ast->value, "|"))
		exit_ast_child(shell, execute_pipeline(shell, ast));
	execute_command_child(shell, ast);
}
