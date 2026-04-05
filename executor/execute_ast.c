/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/04 21:25:41 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"

static char	*make_heredoc_tmp_path(void)
{
	char			*pid_str;
	char			*seq_str;
	char			*base;
	char			*path;
	static size_t	seq;

	pid_str = ft_itoa(getpid());
	seq_str = ft_itoa((int)seq++);
	if (!pid_str || !seq_str)
		return (free(pid_str), free(seq_str), NULL);
	base = ft_strjoin(HERE_DOC_TMP "_", pid_str);
	free(pid_str);
	if (!base)
		return (free(seq_str), NULL);
	path = ft_strjoin(base, "_");
	free(base);
	if (!path)
		return (free(seq_str), NULL);
	base = path;
	path = ft_strjoin(base, seq_str);
	free(base);
	free(seq_str);
	return (path);
}

static int	read_heredoc_to_path(char *limiter, char *path)
{
	int		fd;
	char	*line;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(path), 1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	if (line)
		free(line);
	close(fd);
	return (0);
}

static int	preprocess_command_heredocs(t_ast *ast)
{
	t_redir	*redir;
	char	*limiter;
	char	*path;

	redir = ast->redirs;
	while (redir)
	{
		if (redir->type == TOK_RDIR_HEREDOC)
		{
			limiter = strip_quotes(redir->file);
			path = make_heredoc_tmp_path();
			if (!limiter || !path || read_heredoc_to_path(limiter, path))
				return (free(limiter), free(path), 1);
			free(limiter);
			free(redir->file);
			redir->file = path;
			redir->type = TOK_RDIR_IN;
		}
		redir = redir->next;
	}
	return (0);
}

static int	preprocess_heredocs(t_ast *ast)
{
	if (!ast)
		return (0);
	if (!ft_strcmp(ast->value, "|"))
		return (preprocess_heredocs(ast->left)
			|| preprocess_heredocs(ast->right));
	return (preprocess_command_heredocs(ast));
}

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
		return (builtin_exit(shell, argv));
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

static void	exit_shell_parent(t_shell *shell)
{
	int	exit_code;

	exit_code = shell->exit_code;
	free_shell(shell);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	exit(exit_code);
}

int	execute_ast(t_shell *shell, t_ast *ast)
{
	char	**argv;
	int		status;

	if (!shell || !ast)
		return (1);
	if (preprocess_heredocs(ast))
		return (1);
	if (!ft_strcmp(ast->value, "|"))
		return (execute_pipeline(shell, ast));
	argv = build_argv(ast->value);
	if (argv && argv[0] && is_parent_builtin(argv[0]))
	{
		status = execute_parent_builtin_node(shell, ast, argv);
		ft_strarr_free(argv);
		if (shell->should_exit)
			exit_shell_parent(shell);
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
