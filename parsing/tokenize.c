/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 15:52:36 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 18:09:31 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell_tokenize.h"

t_token	*create_token(const char *value, size_t len, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strndup(value, len);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->preserve_empty = 0;
	new_token->next = NULL;
	return (new_token);
}

size_t	token_len(const char *input, size_t start)
{
	size_t	len;
	int		single_quote;
	int		double_quote;

	len = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[start + len])
	{
		if (input[start + len] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (input[start + len] == '"' && !single_quote)
			double_quote = !double_quote;
		if (!single_quote && !double_quote && (ft_isspace(input[start + len])
				|| is_operator((char *)&input[start + len])))
			break ;
		len++;
	}
	if (single_quote || double_quote)
		return ((size_t)-1);
	return (len);
}

static int	append_token(t_token **head, t_token **tail, t_token *token)
{
	if (!token)
		return (0);
	if (*tail)
		(*tail)->next = token;
	else
		*head = token;
	*tail = token;
	return (1);
}

static t_token	*read_next_token(const char *input, int *i, t_token *head)
{
	size_t	len;
	size_t	op_len;
	int		type;
	t_token	*token;

	type = get_operator_type_and_length(input, *i, &op_len);
	if (input[*i] == '(' || input[*i] == ')')
		return ((*i)++, create_token(input + *i - 1, 1,
				(input[*i - 1] == '(') * TOK_BRAC_OPEN
				+ (input[*i - 1] == ')') * TOK_BRAC_CLOSE));
	if (type != -1)
		return (*i += op_len, create_token(input + *i - op_len, op_len, type));
	len = token_len(input, *i);
	if (len == (size_t)-1)
		return (ft_putendl_fd("minishell: unmatched quote", 2),
			free_token_list(head), NULL);
	token = create_token(input + *i, len, TOK_WORD);
	if (token)
		token->preserve_empty = has_quotes(token->value);
	*i += len;
	return (token);
}

t_token	*parse_token(const char *input)
{
	t_token	*token;
	t_token	*head;
	t_token	*tail;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	if (!input || !*input)
		return (NULL);
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue ;
		}
		token = read_next_token(input, &i, head);
		if (!append_token(&head, &tail, token))
			return (NULL);
	}
	return (head);
}
