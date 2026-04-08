/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell read_input.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 18:39:33 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 18:41:25 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_char(char *line, size_t len, char c)
{
	char	*new_line;

	new_line = malloc(len + 2);
	if (!new_line)
		return (free(line), NULL);
	if (line)
	{
		ft_memcpy(new_line, line, len);
		free(line);
	}
	new_line[len] = c;
	new_line[len + 1] = '\0';
	return (new_line);
}

char	*read_noninteractive_line(void)
{
	char	c;
	char	*line;
	size_t	len;
	ssize_t	nread;

	line = NULL;
	len = 0;
	while (1)
	{
		nread = read(STDIN_FILENO, &c, 1);
		if (nread <= 0)
			break ;
		if (c == '\n')
			break ;
		line = append_char(line, len, c);
		if (!line)
			return (NULL);
		len++;
	}
	if (nread <= 0 && len == 0)
		return (NULL);
	if (!line)
		line = ft_strdup("");
	return (line);
}
