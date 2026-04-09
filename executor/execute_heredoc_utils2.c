/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 15:26:29 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/09 15:48:48 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin/builtin.h"
#include "../parsing/tokenize.h"
#include "executor.h"

/* Match shell SIGINT status so the parent can restore prompt state. */
static int	get_heredoc_lines(char *limiter, char ***out_lines, int *line_count)
{
	int		count;
	char	**lines;

	lines = collect_heredoc_lines(limiter, &count);
	if (!lines)
	{
		if (g_signal == SIGINT)
			return (130);
		return (1);
	}
	*out_lines = lines;
	*line_count = count;
	return (0);
}

static int	write_heredoc_line(t_shell *shell, char *line, int should_expand,
		int fd)
{
	char	*expanded;

	if (should_expand)
	{
		expanded = expand_heredoc_line(line, shell);
		if (!expanded)
			return (1);
		write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	return (0);
}

int	read_heredoc_to_path(t_shell *shell, char *limiter, char *path,
		int should_expand)
{
	int		fd;
	char	**lines;
	int		count;
	int		i;
	int		status;

	status = get_heredoc_lines(limiter, &lines, &count);
	if (status != 0)
		return (status);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(path), free_lines(lines, count), 1);
	i = 0;
	while (i < count)
	{
		if (write_heredoc_line(shell, lines[i], should_expand, fd))
			return (free_lines(lines, count), close(fd), 1);
		i++;
	}
	return (free_lines(lines, count), close(fd), 0);
}
