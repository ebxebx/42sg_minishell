/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:09:47 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:49:47 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int	validate_redirection(t_token *cur)
{
	if (is_redirection_type(cur->type))
	{
		if (!cur->next)
			return (print_syntax_error(NULL), 1);
		if (cur->next->type != TOK_WORD)
			return (print_syntax_error(cur->next->value), 1);
	}
	else if (cur->type == TOK_PIPE)
	{
		if (!cur->next)
			return (print_syntax_error(NULL), 1);
		if (!is_command_start(cur->next->type))
			return (print_syntax_error(cur->next->value), 1);
	}
	return (0);
}

int	validate_tokens(t_token *tokens)
{
	t_token	*cur;

	if (!tokens)
		return (0);
	cur = tokens;
	if (cur->type == TOK_PIPE || is_unsupported_type(cur->type))
		return (print_syntax_error(cur->value));
	while (cur)
	{
		if (is_unsupported_type(cur->type))
			return (print_syntax_error(cur->value));
		if (validate_redirection(cur))
			return (0);
		cur = cur->next;
	}
	return (1);
}
