/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 17:28:01 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:24:40 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "tokenize.h"

int	get_operator_type_and_length(const char *input, size_t start,
		size_t *op_len)
{
	if (ft_strncmp(input + start, "||", 2) == 0)
		return (*op_len = 2, TOK_OR);
	else if (ft_strncmp(input + start, "&&", 2) == 0)
		return (*op_len = 2, TOK_AND);
	else if (ft_strncmp(input + start, "<<", 2) == 0)
		return (*op_len = 2, TOK_RDIR_HEREDOC);
	else if (ft_strncmp(input + start, ">>", 2) == 0)
		return (*op_len = 2, TOK_RDIR_APPEND);
	else if (ft_strncmp(input + start, ">|", 2) == 0)
		return (*op_len = 2, TOK_RDIR_OUT);
	else if (input[start] == '|')
		return (*op_len = 1, TOK_PIPE);
	else if (input[start] == '>')
		return (*op_len = 1, TOK_RDIR_OUT);
	else if (input[start] == '<')
		return (*op_len = 1, TOK_RDIR_IN);
	return (-1);
}

int	has_quotes(const char *value)
{
	int	i;

	i = 0;
	while (value && value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

void	print_token_list(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		ft_printf("Token: %s, Type: %d\n", current->value, current->type);
		current = current->next;
	}
}

void	free_token_list(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
