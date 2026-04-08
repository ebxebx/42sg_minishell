/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 17:53:44 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:21:15 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function to create a new AST node
t_ast	*create_node(char *value)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	if (value)
		value = ft_strdup(value);
	else
		value = ft_strdup("");
	new_node->value = value;
	new_node->argv = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->redirs = NULL;
	return (new_node);
}

char	**append_argv_word(char **argv, const char *word)
{
	char	**new_argv;
	size_t	count;

	count = 0;
	while (argv && argv[count])
		count++;
	new_argv = ft_calloc(count + 2, sizeof(char *));
	if (!new_argv)
		return (NULL);
	count = 0;
	while (argv && argv[count])
	{
		new_argv[count] = argv[count];
		count++;
	}
	new_argv[count] = ft_strdup(word);
	if (!new_argv[count])
		return (free(new_argv), NULL);
	new_argv[count + 1] = NULL;
	free(argv);
	return (new_argv);
}

char	**append_split_argv_words(char **argv, const char *word)
{
	size_t	start;
	size_t	len;
	char	*part;

	start = 0;
	while (word && word[start])
	{
		while (word[start] && is_word_sep(word[start]))
			start++;
		if (!word[start])
			break ;
		len = 0;
		while (word[start + len] && !is_word_sep(word[start + len]))
			len++;
		part = ft_strndup(word + start, len);
		if (!part)
			return (ft_strarr_free(argv), NULL);
		argv = append_argv_word(argv, part);
		free(part);
		if (!argv)
			return (NULL);
		start += len;
	}
	return (argv);
}

/* Main function for testing */
/*
int	main(void)
{
	const char	*input = "echo 'Hello World' | grep Hello > output.txt | wc -l";
	t_token		*tokens;
	t_ast		*ast;

	// const char	*input = "(echo 'Hello World' | grep Hello > output.txt)
			&& (cat input.txt | sort | uniq)";
	tokens = parse_token(input);
	ast = parse_tokens_to_ast(tokens);
	ft_printf("AST:\n");
	print_ast(ast, 0);
	free_ast(ast);
	return (0);
}
*/

// Example usage
/* int main(void) {
	t_ast *root = create_node("root");
	root->left = create_node("left");
	root->right = create_node("right");
	free_ast(root);
	return (0);
} */
