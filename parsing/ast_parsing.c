/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:09:24 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 20:18:58 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int	handle_redirection(t_cmd_parser *cmd)
{
	if (!cmd->cur->next || cmd->cur->next->type != TOK_WORD)
		return (1);
	if (append_redir(cmd->ast, cmd->cur->type, cmd->cur->next->value,
			cmd->cur->next->preserve_empty))
		return (1);
	cmd->cur = cmd->cur->next->next;
	return (0);
}

static int	consume_cmd_token(t_cmd_parser *cmd)
{
	if (cmd->cur->type == TOK_WORD && is_digits_only(cmd->cur->value)
		&& cmd->cur->glued_right && cmd->cur->next
		&& is_redirection_type(cmd->cur->next->type))
	{
		cmd->cur = cmd->cur->next;
		return (0);
	}
	if (!cmd->cur->value[0] && !cmd->cur->preserve_empty)
	{
		cmd->cur = cmd->cur->next;
		return (0);
	}
	if (!cmd->cur->preserve_empty)
		cmd->ast->argv = append_split_argv_words(cmd->ast->argv,
				cmd->cur->value);
	else
		cmd->ast->argv = append_argv_word(cmd->ast->argv, cmd->cur->value);
	if (!cmd->ast->argv)
		return (1);
	cmd->cur = cmd->cur->next;
	return (0);
}

static int	finalize_cmd_node(t_cmd_parser *cmd)
{
	if (!cmd->ast->argv || !cmd->ast->argv[0])
		return (0);
	free(cmd->ast->value);
	cmd->ast->value = ft_strdup(cmd->ast->argv[0]);
	if (!cmd->ast->value)
		return (1);
	return (0);
}

t_ast	*parse_cmd(t_token **tokens)
{
	t_cmd_parser	cmd;

	if (!tokens || !*tokens)
		return (NULL);
	cmd.cur = *tokens;
	cmd.ast = create_node("");
	if (!cmd.ast)
		return (NULL);
	while (cmd.cur && ft_strcmp(cmd.cur->value, "|") != 0)
	{
		if (is_redirection_type(cmd.cur->type))
		{
			if (handle_redirection(&cmd))
				return (free_ast(cmd.ast), NULL);
			continue ;
		}
		if (consume_cmd_token(&cmd))
			return (free_ast(cmd.ast), NULL);
	}
	if (finalize_cmd_node(&cmd))
		return (free_ast(cmd.ast), NULL);
	*tokens = cmd.cur;
	return (cmd.ast);
}

t_ast	*parse_pipeline(t_token *tokens)
{
	t_token	*cur;
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;

	cur = tokens;
	left = parse_cmd(&cur);
	if (!left)
		return (NULL);
	while (cur && ft_strcmp(cur->value, "|") == 0)
	{
		cur = cur->next;
		right = parse_cmd(&cur);
		if (!right)
			return (free_ast(left), NULL);
		node = create_node("|");
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}
