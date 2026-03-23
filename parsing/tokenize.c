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

size_t token_len(const char *input, size_t start)
{
	size_t	len = 0;
	int	single_quote = 0;
	int double_quote = 0;

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
		return (-1); // Unmatched quote error
	return (len);
}

t_token	*parse_token(const char *input)
{
	t_token	*token;
	t_token	*first_token;
	t_token	*last_token;

	first_token = NULL;
	last_token = NULL;
	int		single_quote;
	int		quote_open;

	int		i;
	size_t	len;

	single_quote = 0;
	quote_open = 0;
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
				// Handle unmatched quote error
				printf("Error: Unmatched quote\n");
				return (NULL);
			}
			token = create_token(input + i, len, TOKEN_WORD);
			i += len; // Move index to end of token
		}

		if (!token)
			return (NULL);
		if (last_token)
			last_token->next = token;
		if (!first_token)
			first_token = token;
		last_token = token;
		print_token_list(last_token);
	}
	if (last_token)
		last_token->next = NULL;
	return (first_token);
}

void print_token_list(t_token *tokens)
{
	t_token *current = tokens;
	while (current)
	{
		printf("Token: %s, Type: %d\n", current->value, current->type);
		current = current->next;
	}
}

void free_token_list(t_token *head)
{
	t_token *current = head;
	t_token *next;

	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

// int main(void)
// {
// 	const char *input = "echo 'Hello World'\"!\" | grep Hello ||<><<>>";
// 	t_token *tokens = parse_token(input);
// 	print_token_list(tokens);
// 	free_token_list(tokens);
// 	return (0);
// }