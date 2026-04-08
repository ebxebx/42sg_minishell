/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:05:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:13:34 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	is_word_sep(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

// Function to free the AST
void	free_ast(t_ast *node)
{
	if (node)
	{
		free_ast(node->left);
		free_ast(node->right);
		free_redirs(node->redirs);
		ft_strarr_free(node->argv);
		free(node->value);
		free(node);
	}
}
