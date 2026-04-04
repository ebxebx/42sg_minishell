/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/04 15:30:09 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"

static int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit"));
}

static int	execute_parent_builtin(t_shell *shell, char **argv)
{
	if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(shell, argv));
	if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(shell, argv));
	if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd(argv));
	if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit(shell));
	return (1);
}

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

int	execute_ast(t_shell *shell, t_ast *ast)
{
	char	**argv;
	int		status;

	if (!shell || !ast)
		return (1);
	if (!ft_strcmp(ast->value, "|"))
		return (execute_pipeline(shell, ast));
	argv = build_argv(ast->value);
	if (argv && argv[0] && is_parent_builtin(argv[0]))
	{
		status = execute_parent_builtin_node(shell, ast, argv);
		ft_strarr_free(argv);
		return (status);
	}
	ft_strarr_free(argv);
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
