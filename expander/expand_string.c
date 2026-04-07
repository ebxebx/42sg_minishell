/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:26 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/07 10:28:05 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	copy_str(char **res, char *add)
{
	char	*new_str;
	int		j;
	int		i;

	if (!res || !*res || !add)
		return (0);
	new_str = malloc(sizeof(char) * (ft_strlen(*res) + ft_strlen(add) + 1));
	if (!new_str)
		return (0);
	i = 0;
	while ((*res)[i])
	{
		new_str[i] = (*res)[i];
		i++;
	}
	j = 0;
	while (add[j])
		new_str[i++] = add[j++];
	new_str[i] = '\0';
	free(*res);
	*res = new_str;
	return (1);
}

// null terminate so that can reuse copy_str
int	copy_char(char **res, char c)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	return (copy_str(res, tmp));
}

static int	is_quote_char(char c, int squot, int dquot)
{
	if (c == '\'' && dquot == 0)
		return (1);
	if (c == '"' && squot == 0)
		return (1);
	return (0);
}

//if current char is $,try expand, else copy the current char into res
static int	proc_expand_or_char(char *src, int *i, t_shell *shell, char **res)
{
	if (src[*i] == '$')
		return (handle_expansion(src, i, shell, res));
	return (copy_char(res, src[*i]));
}

char	*expand_string(char *src, t_shell *shell)
{
	int		i;
	int		squot;
	int		dquot;
	char	*res;

	i = 0;
	squot = 0;
	dquot = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (src && src[i])
	{
		if (is_quote_char(src[i], squot, dquot))
			update_quote_stat(src[i], &squot, &dquot);
		else if (squot == 0 && proc_expand_or_char(src, &i, shell, &res) == 0)
			return (free(res), NULL);
		else if (squot != 0 && copy_char(&res, src[i]) == 0)
			return (free(res), NULL);
		i++;
	}
	return (res);
}
