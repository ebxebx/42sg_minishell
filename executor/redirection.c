/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 23:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/01 19:43:53 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	apply_redirections(t_ast *cmd)
{
	int		fd;
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN
			|| redir->type == TOKEN_REDIRECT_HEREDOC)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == TOKEN_REDIRECT_APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (perror(redir->file), 1);
		if ((redir->type == TOKEN_REDIRECT_IN
				|| redir->type == TOKEN_REDIRECT_HEREDOC)
			&& dup2(fd, STDIN_FILENO) < 0)
			return (close(fd), perror(redir->file), 1);
		if ((redir->type == TOKEN_REDIRECT_OUT
				|| redir->type == TOKEN_REDIRECT_APPEND)
			&& dup2(fd, STDOUT_FILENO) < 0)
			return (close(fd), perror(redir->file), 1);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
