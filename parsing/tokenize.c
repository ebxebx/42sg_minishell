/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 15:52:36 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/23 20:02:20 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_tokenize.h"

t_token	*create_token(const char *value, size_t len, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = strndup(value, len);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
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
		if (input[start + len] == '\'')
			single_quote = !single_quote;
		else if (input[start + len] == '\"')
			double_quote = !double_quote;
		if (!single_quote && !double_quote && isspace(input[start + len]))
			break;
		len++;
	}
	if (single_quote || double_quote)
		return ((size_t)-1);
	return (len);
}

t_token	*parse_token(const char *input)
{
	t_token	*token;
	t_token	*first_token;
	t_token	*last_token;
	int		i;
	size_t	len;

	first_token = NULL;
	last_token = NULL;
	i = 0;
	if (!input || !*input)
		return (NULL);
	while (input[i])
	{
		if (isspace(input[i]))
		{
			i++;
			continue;
		}
		if (input[i] == '(')
		{
			token = create_token(input + i, 1, TOKEN_PARANTHESIS_OPEN);
			i++;
		}
		else if (input[i] == ')')
		{
			token = create_token(input + i, 1, TOKEN_PARANTHESIS_CLOSE);
			i++;
		}
		else if (strncmp(input + i, "||", 2) == 0)
		{
			token = create_token(input + i, 2, TOKEN_OR);
			i += 2;
		}
		else if (strncmp(input + i, "&&", 2) == 0)
		{
			token = create_token(input + i, 2, TOKEN_AND);
			i += 2;
		}
		else if (strncmp(input + i, "<<", 2) == 0)
		{
			token = create_token(input + i, 2, TOKEN_REDIRECT_HEREDOC);
			i += 2;
		}
		else if (strncmp(input + i, ">>", 2) == 0)
		{
			token = create_token(input + i, 2, TOKEN_REDIRECT_APPEND);
			i += 2;
		}
		else if (input[i] == '|')
		{
			token = create_token(input + i, 1, TOKEN_PIPE);
			i++;
		}
		else if (input[i] == '>')
		{
			token = create_token(input + i, 1, TOKEN_REDIRECT_OUT);
			i++;
		}
		else if (input[i] == '<')
		{
			token = create_token(input + i, 1, TOKEN_REDIRECT_IN);
			i++;
		}
		else
		{
			len = token_len(input, i);
			if (len == (size_t)-1)
			{
				printf("Error: Unmatched quote\n");
				free_token_list(first_token);
				return (NULL);
			}
			token = create_token(input + i, len, TOKEN_WORD);
			i += len;
		}
		if (!token)
		{
			free_token_list(first_token);
			return (NULL);
		}
		if (last_token)
			last_token->next = token;
		if (!first_token)
			first_token = token;
		last_token = token;
	}
	return (first_token);
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	free(token->value);
	free(token);
}

void	print_token_list(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		printf("Token: %s, Type: %d\n", current->value, current->type);
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
