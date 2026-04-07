/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:26 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/07 10:27:35 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

//scans left to right, and quote state can change as it goes.
void	update_quote_stat(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && *in_dquote == 0)
		*in_squote = !(*in_squote);
	else if (c == '"' && *in_squote == 0)
		*in_dquote = !(*in_dquote);
}

int	handle_expansion(char *src, int *i, t_shell *shell, char **res)
{
	if (!src[*i + 1])
		return (copy_char(res, '$'));
	if (src[*i + 1] == '?')
	{
		*i += 1;
		return (expand_exit_status(shell, res));
	}
	return (expand_env_var(src, i, shell, res));
}
