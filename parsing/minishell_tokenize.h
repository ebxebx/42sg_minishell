/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokenize.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:09:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/31 15:54:31 by ka-tan           ###   ########.fr       */
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
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_HEREDOC,
	TOKEN_REDIRECT_APPEND,
	TOKEN_PARANTHESIS_OPEN,
	TOKEN_PARANTHESIS_CLOSE,
	TOKEN_AND,
	TOKEN_OR,
	// Add other token types
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

t_token		*create_token(const char *value, size_t len, t_token_type type);
void		free_token(t_token *token);
void		free_token_list(t_token *head);
void		print_token_list(t_token *head);
t_token		*parse_token(const char *input);

#endif