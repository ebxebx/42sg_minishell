/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:26 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/04 13:31:29 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	expand_tokens(t_token *tokens, t_shell *shell)
{
	char	*expanded;
	int		skip_heredoc_word;

	skip_heredoc_word = 0;
	while (tokens)
	{
		if (tokens->type == TOK_RDIR_HEREDOC)
			skip_heredoc_word = 1;
		else if (tokens->type == TOK_WORD && skip_heredoc_word)
			skip_heredoc_word = 0;
		else if (tokens->type == TOK_WORD)
		{
			expanded = expand_string(tokens->value, shell);
			if (!expanded)
				return (0);
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
	return (1);
}
