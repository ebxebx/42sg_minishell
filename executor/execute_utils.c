/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:29:01 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/07 19:09:08 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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
	out = malloc(ft_strlen(value) + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (value[i])
	{
		if (value[i] == '\'' && double_quote == 0)
				single_quote = !single_quote;
		else if (value[i] == '\"' && single_quote == 0)
			double_quote = !double_quote;

		if (
			(single_quote && !double_quote && value[i] != '\'')
			|| (double_quote && !single_quote && value[i] != '"')
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
