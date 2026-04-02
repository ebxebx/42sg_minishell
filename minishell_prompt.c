/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 13:49:54 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/02 15:26:47 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_prompt.h"

char	*build_prompt(int status)
{
	const char	*pre = "minishell[";
	const char	*post = "]> ";
	char		*num;
	char		*prompt;
	size_t		len;

	num = ft_itoa(status);
	if (!num)
		return (NULL);
	len = ft_strlen(pre) + ft_strlen(num) + ft_strlen(post) + 1;
	prompt = malloc(len);
	if (!prompt)
	{
		free(num);
		return (NULL);
	}
	ft_strlcpy(prompt, pre, len);
	ft_strlcat(prompt, num, len);
	ft_strlcat(prompt, post, len);
	free(num);
	return (prompt);
}
