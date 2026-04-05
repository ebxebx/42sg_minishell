/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:26 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/05 11:23:04 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

//get the length of the variable bef an invalid char
static int	var_key_len(char *src, int start)
{
	int	len;

	len = 0;
	if (!src[start])
		return (0);
	if (!ft_isalpha(src[start]) && src[start] != '_')
		return (0);
	while (src[start + len]
		&& (ft_isalnum(src[start + len]) || src[start + len] == '_'))
		len++;
	return (len);
}

int	expand_exit_status(t_shell *shell, char **res)
{
	char	*tmp;
	int		ok;

	tmp = ft_itoa(shell->status);
	if (!tmp)
		return (0);
	ok = copy_str(res, tmp);
	free(tmp);
	return (ok);
}

int	expand_env_var(char *src, int *i, t_shell *shell, char **res)
{
	char	*key;
	char	*value;
	int		len;

	len = var_key_len(src, *i + 1);
	if (len == 0)
		return (copy_char(res, '$'));
	key = ft_substr(src, *i + 1, len);
	if (!key)
		return (0);
	value = get_env_value(shell->env, key);
	if (value && copy_str(res, value) == 0)
		return (free(key), 0);
	*i += len;
	free(key);
	return (1);
}
