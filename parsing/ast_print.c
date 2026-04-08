/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 19:09:43 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 19:42:22 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (!token)
		ft_putstr_fd("newline", 2);
	else
		ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
	return (0);
}

static void	print_ast_argv(char **argv)
{
	int	i;

	if (!argv || !argv[0])
	{
		ft_printf("%s", "");
		return ;
	}
	ft_printf("[");
	i = 0;
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(", ");
		i++;
	}
	ft_printf("]");
}

static void	print_redir_type(t_redir *redir)
{
	if (redir->type == TOK_RDIR_IN)
		ft_printf("< %s", redir->file);
	else if (redir->type == TOK_RDIR_OUT)
		ft_printf("> %s", redir->file);
	else if (redir->type == TOK_RDIR_APPEND)
		ft_printf(">> %s", redir->file);
	else if (redir->type == TOK_RDIR_HEREDOC)
		ft_printf("<< %s", redir->file);
}

static void	print_redirs(t_redir *redir)
{
	if (!redir)
		return ;
	ft_printf(" [");
	while (redir)
	{
		print_redir_type(redir);
		if (redir->next)
			ft_printf(", ");
		redir = redir->next;
	}
	ft_printf("]");
}

/* Print AST (for debugging purposes) */
void	print_ast(t_ast *node, int depth)
{
	int		i;

	if (!node)
		return ;
	if (depth == 0)
		ft_printf("AST:\n");
	print_ast(node->right, depth + 1);
	i = 0;
	while (i++ < depth)
		ft_printf("    ");
	if (node->argv)
		print_ast_argv(node->argv);
	else
		ft_printf("%s", node->value);
	print_redirs(node->redirs);
	ft_printf("\n");
	print_ast(node->left, depth + 1);
}
