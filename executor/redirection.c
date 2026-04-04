/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 23:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/03 20:10:47 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	is_heredoc_tmp_file(const char *path)
{
	size_t	prefix_len;

	if (!path)
		return (0);
	prefix_len = ft_strlen(HERE_DOC_TMP "_");
	return (ft_strncmp(path, HERE_DOC_TMP "_", prefix_len) == 0);
}

int	apply_redirections(t_ast *cmd)
{
	int		fd;
	t_redir	*redir;
	char	*target;

	redir = cmd->redirs;
	while (redir)
	{
		target = strip_quotes(redir->file);
		if (!target)
			return (1);
		if (redir->type == TOK_RDIR_IN)
			fd = open(target, O_RDONLY);
		else if (redir->type == TOK_RDIR_HEREDOC)
			return (free(target), 1);
		else if (redir->type == TOK_RDIR_APPEND)
			fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (free(target), perror(target), 1);
		if (redir->type == TOK_RDIR_IN && is_heredoc_tmp_file(target))
			unlink(target);
		if (redir->type == TOK_RDIR_IN && dup2(fd, STDIN_FILENO) < 0)
			return (close(fd), free(target), perror(target), 1);
		if ((redir->type == TOK_RDIR_OUT || redir->type == TOK_RDIR_APPEND)
			&& dup2(fd, STDOUT_FILENO) < 0)
			return (close(fd), free(target), perror(target), 1);
		close(fd);
		free(target);
		redir = redir->next;
	}
	return (0);
}
