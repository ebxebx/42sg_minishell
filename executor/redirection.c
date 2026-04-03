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

// return fd
int	read_here_doc(char *limiter)
{
	int		fd;
	char	*line;

	ft_printf("read_here_doc\n");
	fd = open(HERE_DOC_TMP, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	while (1)
	{
		line = readline("> ");
		if (ft_strcmp(line, limiter) != 0)
			write(fd, line, ft_strlen(line));
		else
			break ;
		free(line);
	}
	if (line)
		free(line);
	close(fd);
	fd = open(HERE_DOC_TMP, O_RDONLY);
	return (unlink(HERE_DOC_TMP), fd);
}

int	apply_redirections(t_ast *cmd)
{
	int		fd;
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOK_RDIR_IN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == TOK_RDIR_HEREDOC)
			fd = read_here_doc(redir->file);
		else if (redir->type == TOK_RDIR_APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (perror(redir->file), 1);
		if (redir->type == TOK_RDIR_IN && dup2(fd, STDIN_FILENO) < 0)
			return (close(fd), perror(redir->file), 1);
		if (redir->type == TOK_RDIR_HEREDOC	&& dup2(fd, STDIN_FILENO) < 0)
			return (close(fd), perror(redir->file), 1);
		if ((redir->type == TOK_RDIR_OUT || redir->type == TOK_RDIR_APPEND)
			&& dup2(fd, STDOUT_FILENO) < 0)
			return (close(fd), perror(redir->file), 1);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
