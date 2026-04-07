/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:29:01 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/07 17:59:39 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

//scans left to right, and quote state can change as it goes.
/* static void	update_quote_stat(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && *in_dquote == 0)
		*in_squote = !(*in_squote);
	else if (c == '"' && *in_squote == 0)
		*in_dquote = !(*in_dquote);
	} */

void	close_all_fds(void)
{
	int	fd;
	int	probe_fd;

	fd = 0;
	while (fd < 1024)
	{
		probe_fd = dup(fd);
		if (!(probe_fd < 0 && errno == EBADF))
		{
			if (probe_fd >= 0)
				close(probe_fd);
			close(fd);
		}
		fd++;
	}
}

char	*strip_quotes(const char *value)
{
	size_t	i;
	size_t	j;
	char	*out;
	int	single_quote;
	int double_quote;

	single_quote = 0;
	double_quote = 0;

/* 	while (input[start + len])
	{
		// KS: Quote handling only happen when not alrd inside the other quote type
		if (input[start + len] == '\'' && double_quote == 0)
				single_quote = !single_quote;
		else if (input[start + len] == '\"' && single_quote == 0)
			double_quote = !double_quote;

		if (!single_quote && !double_quote && ft_isspace(input[start + len]))
			break;
		// KS: need to check for operator only when not inside any quote, otherwise it will break tokenization of something like "echo '|'"
		if (!single_quote && !double_quote
			&& is_operator((char *)&input[start + len]))
			break;
		len++;
	} */


	out = malloc(ft_strlen(value) + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (value[i])
	{
		// KS: Quote handling only happen when not alrd inside the other quote type
		if (value[i] == '\'' && double_quote == 0)
				single_quote = !single_quote;
		else if (value[i] == '\"' && single_quote == 0)
			double_quote = !double_quote;

		if (
			(single_quote && !double_quote && value[i] != '\'')  // remove single quotes
			|| (double_quote && !single_quote && value[i] != '"') // remove double quotes
			|| (!single_quote && !double_quote && (value[i] != '\'' && value[i] != '"'))
		)
			out[j++] = value[i];
		i++;
	}
	out[j] = '\0';
	return (out);
}

/* char	**build_argv(t_shell *shell, char *cmdline)
{
	t_token *tokens;
	t_token *cur;
	char **argv;
	size_t count;
	size_t i;

	tokens = parse_token(cmdline);
	if (!tokens)
		return (NULL);
	count = 0;
	cur = tokens;
	while (cur && ++count)
		cur = cur->next;
	argv = ft_calloc(count + 1, sizeof(char *));
	if (!argv)
		return (free_token_list(tokens), NULL);
	cur = tokens;
	i = 0;
	while (cur)
	{
		argv[i] = strip_quotes(cur->value);
		if (shell->debug)
			ft_printf("Token %zu: %s ->", i, argv[i]);
		if (!argv[i++])
			return (free_token_list(tokens), ft_strarr_free(argv), NULL);
		cur = cur->next;
	}
	if (shell->debug)
		ft_printf("\n");
	free_token_list(tokens);
	return (argv);
} */
