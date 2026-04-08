/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:10:35 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:11:22 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->file);
		free(redir);
		redir = next;
	}
}

int	append_redir(t_ast *ast, t_token_type type, const char *file,
		int preserve_empty)
{
	t_redir	*new_redir;
	t_redir	*cur;

	if (!ast || !file)
		return (1);
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (1);
	new_redir->type = type;
	new_redir->file = ft_strdup(file);
	new_redir->preserve_empty = preserve_empty;
	new_redir->next = NULL;
	if (!new_redir->file)
		return (free(new_redir), 1);
	if (!ast->redirs)
	{
		ast->redirs = new_redir;
		return (0);
	}
	cur = ast->redirs;
	while (cur->next)
		cur = cur->next;
	cur->next = new_redir;
	return (0);
}
