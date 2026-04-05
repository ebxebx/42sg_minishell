/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:34:21 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/05 11:23:09 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../minishell.h"
# include "../parsing/ast.h"
# include "../libft/libft.h"
# include "../env/env.h"

int		expand_exit_status(t_shell *shell, char **res);
int		expand_env_var(char *src, int *i, t_shell *shell, char **res);
char	*expand_string(char *src, t_shell *shell);
int		expand_tokens(t_token *tokens, t_shell *shell);
int		copy_str(char **res, char *add);
int		copy_char(char **res, char c);
void	update_quote_stat(char c, int *in_squote, int *in_dquote);

#endif
