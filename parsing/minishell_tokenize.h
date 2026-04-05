/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:09:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/03 15:09:25 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TOKENIZE_H
# define MINISHELL_TOKENIZE_H

# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_RDIR_IN,
	TOK_RDIR_OUT,
	TOK_RDIR_HEREDOC,
	TOK_RDIR_APPEND,
	TOK_BRAC_OPEN,
	TOK_BRAC_CLOSE,
	TOK_AND,
	TOK_OR,
	// Add other token types
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				preserve_empty;
	struct s_token	*next;
}					t_token;

t_token		*create_token(const char *value, size_t len, t_token_type type);
void		free_token(t_token *token);
void		free_token_list(t_token *head);
void		print_token_list(t_token *head);
t_token		*parse_token(const char *input);

#endif
