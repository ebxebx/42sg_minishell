/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 23:00:00 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 13:34:39 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	has_whitespace(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == ' ' || str[i] == '\f' || str[i] == '\n' || str[i] == '\r'
			|| str[i] == '\t' || str[i] == '\v')
			return (1);
		i++;
	}
	return (0);
}

static int	is_ambiguous_redirect(t_redir *redir, const char *target)
{
	if (redir->preserve_empty)
		return (0);
	if (!target || !target[0])
		return (1);
	return (has_whitespace(target));
}

int	is_heredoc_tmp_file(const char *path)
{
	size_t	prefix_len;

	if (!path)
		return (0);
	prefix_len = ft_strlen(HERE_DOC_TMP "_");
	return (ft_strncmp(path, HERE_DOC_TMP "_", prefix_len) == 0);
}

// static int	open_fd(t_redir *redir, const char *target, int *fd)
// {
// 	if (redir->type == TOK_RDIR_IN)
// 		*fd = open(target, O_RDONLY);
// 	else if (redir->type == TOK_RDIR_APPEND)
// 		*fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else
// 		*fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	return (*fd == -1);
// }

// static int	dup2_fd(t_redir *redir, int fd, char *target)
// {
// 	if (redir->type == TOK_RDIR_IN && is_heredoc_tmp_file(target))
// 		unlink(target);
// 	if (redir->type == TOK_RDIR_IN && dup2(fd, STDIN_FILENO) < 0)
// 		return (1);
// 	if ((redir->type == TOK_RDIR_OUT || redir->type == TOK_RDIR_APPEND)
// 		&& dup2(fd, STDOUT_FILENO) < 0)
// 		return (1);
// 	return (0);
// }

static int	redirect_fd(t_redir *redir, const char *target)
{
	int	fd;

	if (redir->type == TOK_RDIR_IN)
		fd = open(target, O_RDONLY);
	else if (redir->type == TOK_RDIR_APPEND)
		fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (1);
	if (redir->type == TOK_RDIR_IN && is_heredoc_tmp_file((char *)target))
		unlink(target);
	if (redir->type == TOK_RDIR_IN && dup2(fd, STDIN_FILENO) < 0)
		return (close(fd), 1);
	if ((redir->type == TOK_RDIR_OUT || redir->type == TOK_RDIR_APPEND)
		&& dup2(fd, STDOUT_FILENO) < 0)
		return (close(fd), 1);
	return (close(fd), 0);
}

int	apply_redirections(t_ast *cmd)
{
	t_redir	*redir;
	char	*target;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOK_RDIR_HEREDOC)
			return (1);
		target = strip_quotes(redir->file);
		if (!target)
			return (1);
		if (is_ambiguous_redirect(redir, target))
			return (ft_putendl_fd("minishell: ambiguous redirect", 2),
				free(target), 1);
		if (redirect_fd(redir, target))
			return (perror(target), free(target), 1);
		free(target);
		redir = redir->next;
	}
	return (0);
}
