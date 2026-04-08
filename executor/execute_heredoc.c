/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 11:26:08 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 13:44:10 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"

/* static char	*make_heredoc_tmp_path(void)
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
 */
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

static void	exit_heredoc_child(t_shell *shell, int status)
{
	rl_clear_history();
	if (shell)
		free_shell(shell);
	close_all_fds();
	exit(status);
}

int	run_heredoc_child(t_shell *shell, char *limiter,
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
		init_signal_heredoc();
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

