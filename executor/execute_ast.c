/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 00:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/07 19:00:43 by ka-tan           ###   ########.fr       */
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

static void	free_lines(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(lines[i++]);
	free(lines);
}

/* Break the child's blocking readline() immediately on Ctrl-C. */
static void	sigint_heredoc_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		g_signal = signo;
		close(STDIN_FILENO);
	}
}

static char	**collect_heredoc_lines(char *limiter, int *out_count)
{
	char	**lines;
	char	**tmp;
	char	*line;
	int		count;
	int		cap;

	count = 0;
	cap = 8;
	lines = malloc(sizeof(char *) * cap);
	if (!lines)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
			return (free(line), free_lines(lines, count), NULL);
		if (!line || ft_strcmp(line, limiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		if (count >= cap)
		{
			cap *= 2;
			tmp = realloc(lines, sizeof(char *) * cap);
			if (!tmp)
				return (free(line), free_lines(lines, count), NULL);
			lines = tmp;
		}
		lines[count++] = line;
	}
	*out_count = count;
	return (lines);
}

static int	read_heredoc_to_path(
			t_shell *shell, char *limiter, char *path, int should_expand)
{
	int		fd;
	char	*expanded;
	char	**lines;
	int		count;
	int		i;

	lines = collect_heredoc_lines(limiter, &count);
	if (!lines)
	{
		/* Match shell SIGINT status so the parent can restore prompt state. */
		if (g_signal == SIGINT)
			return (130);
		return (1);
	}
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(path), free_lines(lines, count), 1);
	i = 0;
	while (i < count)
	{
		if (should_expand)
		{
			expanded = expand_heredoc_line(lines[i], shell);
			if (!expanded)
				return (free_lines(lines, count), close(fd), 1);
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(fd, lines[i], ft_strlen(lines[i]));
		write(fd, "\n", 1);
		i++;
	}
	free_lines(lines, count);
	close(fd);
	return (0);
}

static int	is_heredoc_tmp_file(const char *path)
{
	size_t	prefix_len;

	if (!path)
		return (0);
	prefix_len = ft_strlen(HERE_DOC_TMP "_");
	return (ft_strncmp(path, HERE_DOC_TMP "_", prefix_len) == 0);
}

static void	exit_heredoc_child(t_shell *shell, int status)
{
	rl_clear_history();
	if (shell)
		free_shell(shell);
	close_all_fds();
	exit(status);
}

static int	run_heredoc_child(t_shell *shell, char *limiter,
		char *path, int should_expand)
{
	pid_t	pid;
	int		status;
	int		child_status;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		g_signal = 0;
		signal(SIGINT, sigint_heredoc_handler);
		signal(SIGQUIT, SIG_IGN);
		child_status = read_heredoc_to_path(shell, limiter, path, should_expand);
		/* The child owns its forked copies and must free them before exit. */
		free(limiter);
		free(path);
		exit_heredoc_child(shell, child_status);
	}
	signal(SIGINT, SIG_IGN);
	while (waitpid(pid, &status, 0) < 0)
	{
		if (errno != EINTR)
			break ;
	}
	init_signal_prompt();
	if (WIFSIGNALED(status) || (WIFEXITED(status) && WEXITSTATUS(status) != 0))
	{
		/* Let the caller treat heredoc cancellation like an interactive Ctrl-C. */
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			g_signal = SIGINT;
		unlink(path);
		return (1);
	}
	return (0);
}

static int	preprocess_command_heredocs(t_ast *ast, t_shell *shell)
{
	t_redir	*redir;
	t_redir	*cleanup;
	char	*limiter;
	char	*path;
	int		should_expand;

	redir = ast->redirs;
	while (redir)
	{
		if (redir->type == TOK_RDIR_HEREDOC)
		{
			should_expand = !redir->preserve_empty;
			limiter = strip_quotes(redir->file);
			path = make_heredoc_tmp_path();
			if (!limiter || !path
				|| run_heredoc_child(shell, limiter, path, should_expand))
			{
				free(limiter);
				free(path);
				cleanup = ast->redirs;
				while (cleanup != redir)
				{
					if (cleanup->type == TOK_RDIR_IN
						&& is_heredoc_tmp_file(cleanup->file))
						unlink(cleanup->file);
					cleanup = cleanup->next;
				}
				return (1);
			}
			free(limiter);
			free(redir->file);
			redir->file = path;
			redir->type = TOK_RDIR_IN;
		}
		redir = redir->next;
	}
	return (0);
}

static void	cleanup_heredoc_tmps(t_ast *ast)
{
	t_redir	*redir;

	if (!ast)
		return ;
	if (!ft_strcmp(ast->value, "|"))
	{
		cleanup_heredoc_tmps(ast->left);
		cleanup_heredoc_tmps(ast->right);
		return ;
	}
	redir = ast->redirs;
	while (redir)
	{
		if (redir->type == TOK_RDIR_IN && is_heredoc_tmp_file(redir->file))
			unlink(redir->file);
		redir = redir->next;
	}
}

static int	preprocess_heredocs(t_ast *ast, t_shell *shell)
{
	if (!ast)
		return (0);
	if (!ft_strcmp(ast->value, "|"))
	{
		if (preprocess_heredocs(ast->left, shell))
		{
			cleanup_heredoc_tmps(ast->right);
			return (1);
		}
		if (preprocess_heredocs(ast->right, shell))
		{
			cleanup_heredoc_tmps(ast->left);
			return (1);
		}
		return (0);
	}
	return (preprocess_command_heredocs(ast, shell));
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
		return (builtin_cd(shell, argv));
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
