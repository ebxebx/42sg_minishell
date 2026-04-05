/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:29:06 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/30 16:29:20 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell_tokenize.h"

typedef struct s_redir
{
	t_token_type		type;
	char				*file;
	int					preserve_empty;
	struct s_redir		*next;
}					t_redir;

typedef struct s_ast
{
	char			*value;
	char			**argv;
	struct s_ast	*left;
	struct s_ast	*right;
	t_redir			*redirs;
}					t_ast;

t_ast				*parse_tokens_to_ast(t_token *tokens);
void				free_ast(t_ast *node);
void				print_ast(t_ast *node, int depth);

#endif
