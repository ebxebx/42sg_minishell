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
#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"

static char	*strip_quotes(const char *value)
{
	size_t	i;
	size_t	j;
	char	*out;

	out = malloc(ft_strlen(value) + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (value[i])
	{
		if (value[i] != '\'' && value[i] != '"')
			out[j++] = value[i];
		i++;
	}
	out[j] = '\0';
	return (out);
}

static char	**build_argv(char *cmdline)
{
	t_token	*tokens;
	t_token	*cur;
	char	**argv;
	size_t	count;
	size_t	i;

	tokens = parse_token(cmdline);
	if (!tokens)
		return (NULL);
	count = 0;
	cur = tokens;
	while (cur && ++count)
		cur = cur->next;
	argv = ft_calloc(count + 1, sizeof(char *));
	if (!argv)
		return (free_token_list(tokens), NULL);
	cur = tokens;
	i = 0;
	while (cur)
	{
		argv[i] = strip_quotes(cur->value);
		if (!argv[i++])
			return (free_token_list(tokens), ft_strarr_free(argv), NULL);
		cur = cur->next;
	}
	free_token_list(tokens);
	return (argv);
}

static int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"));
}

static int	execute_parent_builtin(t_shell *shell, char **argv)
{
	if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(shell, argv));
	if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(shell, argv));
	return (1);
}

static int	apply_redirections(t_ast *cmd)
{
	int	fd;

	if (cmd->left)
	{
		fd = open(cmd->left->value, O_RDONLY);
		if (fd < 0 || dup2(fd, STDIN_FILENO) < 0)
			return (perror(cmd->left->value), 1);
		close(fd);
	}
	if (cmd->right)
	{
		fd = open(cmd->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0)
			return (perror(cmd->right->value), 1);
		close(fd);
	}
	return (0);
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
