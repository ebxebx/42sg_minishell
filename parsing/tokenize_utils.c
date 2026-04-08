/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 16:05:50 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:24:40 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "tokenize.h"

/*
	In Bash, the operator >| is a specialized redirection tool used to
	force-overwrite (clobber) a file, even if the shell's noclobber option is
	enabled.
*/
int	is_operator(char *token)
{
	if (!token)
		return (0);
	if (ft_strncmp(token, "||", 2) == 0 || ft_strncmp(token, "&&", 2) == 0
		|| ft_strncmp(token, "<<", 2) == 0
		|| ft_strncmp(token, ">>", 2) == 0
		|| ft_strncmp(token, ">|", 2) == 0
		|| ft_strncmp(token, "|", 1) == 0
		|| ft_strncmp(token, ">", 1) == 0
		|| ft_strncmp(token, "<", 1) == 0)
		return (1);
	return (0);
}

int	is_redirection_type(t_token_type type)
{
	return (type == TOK_RDIR_IN || type == TOK_RDIR_OUT
		|| type == TOK_RDIR_HEREDOC || type == TOK_RDIR_APPEND);
}

int	is_unsupported_type(t_token_type type)
{
	return (type == TOK_AND || type == TOK_OR || type == TOK_BRAC_OPEN
		|| type == TOK_BRAC_CLOSE);
}

int	is_command_start(t_token_type type)
{
	return (type == TOK_WORD || is_redirection_type(type));
}

int	is_digits_only(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
