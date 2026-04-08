/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_preprocess.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 13:37:39 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 14:47:26 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin/builtin.h"
#include "../parsing/minishell_tokenize.h"
#include "executor.h"

static void	cleanup_redirs(t_ast *ast, t_redir *redir)
{
	t_redir	*cleanup;

	cleanup = ast->redirs;
	while (cleanup != redir)
	{
		if (cleanup->type == TOK_RDIR_IN && is_heredoc_tmp_file(cleanup->file))
			unlink(cleanup->file);
		cleanup = cleanup->next;
	}
}

static int	preprocess_command_heredocs(t_ast *ast, t_shell *shell)
{
	t_redir	*redir;
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
				return (free(limiter), free(path),
					cleanup_redirs(ast, redir), 1);
			free(limiter);
			free(redir->file);
			redir->file = path;
			redir->type = TOK_RDIR_IN;
		}
		redir = redir->next;
	}
	return (0);
}

int	preprocess_heredocs(t_ast *ast, t_shell *shell)
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
