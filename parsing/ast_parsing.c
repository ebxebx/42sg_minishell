/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:09:24 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 20:04:55 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
1️⃣ Lowest level: command
Handles:
- words → argv (concatenate into single node->value)
- redirections (attach file nodes to left for input and right for output)
This function consumes tokens by advancing *tokens to the next unconsumed token.
*/
/*
** Ignore fd designators like `2>` as argv words until full fd-aware
** redirection support is added.
*/
t_ast	*parse_cmd(t_token **tokens)
{
	t_token	*p;
	t_ast	*ast;

	if (!tokens || !*tokens)
		return (NULL);
	p = *tokens;
	ast = create_node("");
	if (!ast)
		return (NULL);
	while (p && ft_strcmp(p->value, "|") != 0)
	{
		if (is_redirection_type(p->type))
		{
			if (!p->next || p->next->type != TOK_WORD || append_redir(ast,
					p->type, p->next->value, p->next->preserve_empty))
				return (free_ast(ast),NULL);
			p = p->next->next;
			continue ;
		}
		if (p->type == TOK_WORD && is_digits_only(p->value) && p->next
			&& is_redirection_type(p->next->type))
		{
			p = p->next;
			continue ;
		}
		if (!p->value[0] && !p->preserve_empty)
		{
			p = p->next;
			continue ;
		}
		if (!p->preserve_empty)
			ast->argv = append_split_argv_words(ast->argv, p->value);
		else
			ast->argv = append_argv_word(ast->argv, p->value);
		if (!ast->argv)
			return (free_ast(ast),NULL);
		p = p->next;
	}
	if (ast->argv && ast->argv[0])
	{
		free(ast->value);
		ast->value = ft_strdup(ast->argv[0]);
		if (!ast->value)
			return (free_ast(ast), NULL);
	}
	return (*tokens = p, ast);
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

/* Recursive Function to Parse Tokens into AST */
// TODO: add logical-operator and parenthesis parsing with proper precedence.
// For now,	build pipelines (left-associative). Extend precedence handling.
t_ast	*parse_tokens_to_ast(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (!validate_tokens(tokens))
		return (NULL);
	return (parse_pipeline(tokens));
}
