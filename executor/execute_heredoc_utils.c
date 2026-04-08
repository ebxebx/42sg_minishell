/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 12:50:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:24:40 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../builtin/builtin.h"
#include "../parsing/tokenize.h"

char	*make_heredoc_tmp_path(void)
{
	char			*seq_str;
	char			*path;
	int				fd;
	static size_t	seq;

	while (1)
	{
		seq_str = ft_itoa((int)seq++);
		if (!seq_str)
			return (NULL);
		path = ft_strjoin(HERE_DOC_TMP "_", seq_str);
		free(seq_str);
		if (!path)
			return (NULL);
		fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (fd >= 0)
			return (close(fd), path);
		free(path);
		if (errno != EEXIST)
			return (NULL);
	}
}

void	free_lines(char **lines, int count)
{
	int	i;

	i = 0;
	while (i < count)
		free(lines[i++]);
	free(lines);
}

static int	extend_lines(char ***lines, int count, int *cap)
{
	char	**new_lines;
	int		old_cap;

	if (count >= *cap)
	{
		old_cap = *cap;
		*cap *= 2;
		new_lines = ft_realloc(*lines, sizeof(char *) * old_cap,
				sizeof(char *) * (*cap));
		if (!new_lines)
			return (1);
		*lines = new_lines;
	}
	return (0);
}

char	**collect_heredoc_lines(char *limiter, int *out_count)
{
	char	**lines;
	int		count;
	int		cap;

	count = 0;
	cap = 2;
	lines = malloc(sizeof(char *) * cap);
	if (!lines)
		return (NULL);
	while (1)
	{
		if (extend_lines(&lines, count, &cap))
			return (free_lines(lines, count), NULL);
		lines[count] = readline("> ");
		if (g_signal == SIGINT)
			return (free_lines(lines, count), NULL);
		if (!lines[count] || ft_strcmp(lines[count], limiter) == 0)
		{
			free(lines[count]);
			lines[count] = NULL;
			break ;
		}
		count++;
	}
	return (*out_count = count, lines);
}

void	cleanup_heredoc_tmps(t_ast *ast)
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
