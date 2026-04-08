/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:29:06 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 20:19:29 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "tokenize.h"

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int				preserve_empty;
	struct s_redir	*next;
}					t_redir;

typedef struct s_ast
{
	char			*value;
	char			**argv;
	struct s_ast	*left;
	struct s_ast	*right;
	t_redir			*redirs;
}					t_ast;

typedef struct s_cmd_parser
{
	t_token			*cur;
	t_ast			*ast;
}					t_cmd_parser;

t_ast				*parse_pipeline(t_token *tokens);
t_ast				*parse_tokens_to_ast(t_token *tokens);
void				free_ast(t_ast *node);
void				print_ast(t_ast *node, int depth);
char				**append_argv_word(char **argv, const char *word);
char				**append_split_argv_words(char **argv, const char *word);

// ast_validate.c
int					validate_tokens(t_token *tokens);

// ast_utils.c
int					is_word_sep(char c);
void				free_ast(t_ast *node);

// ast_redirs.c
void				free_redirs(t_redir *redir);
int					append_redir(t_ast *ast, t_token_type type,
						const char *file, int preserve_empty);

// ast.c
t_ast				*create_node(char *value);

// ast_print.c
void				print_ast(t_ast *node, int depth);
int					print_syntax_error(char *token);
#endif
