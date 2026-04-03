/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:34:21 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/03 21:19:00 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../minishell.h"
# include "../parsing/ast.h"
# include "../libft/libft.h"
# include "../env/env.h"

int	expand_exit_status(t_shell *shell, char **res);
int	expand_env_var(char *src, int *i, t_shell *shell, char **res);



#endif
