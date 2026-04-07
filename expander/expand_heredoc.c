/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:26 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/07 10:22:24 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

char	*expand_heredoc_line(char *src, t_shell *shell)
{
	int		i;
	char	*res;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (src && src[i])
	{
		if (src[i] == '$')
		{
			if (handle_expansion(src, &i, shell, &res) == 0)
				return (free(res), NULL);
		}
		else if (copy_char(&res, src[i]) == 0)
			return (free(res), NULL);
		i++;
	}
	return (res);
}
