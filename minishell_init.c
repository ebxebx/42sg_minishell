/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/29 13:58:21 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env/env.h"
#include "libft/libft.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->status = 0;
	shell->env = dup_env(env);
	if (!shell->env)
		exit(EXIT_FAILURE);
}

void	free_shell(t_shell *shell)
{
	ft_strarr_free(shell->env);
}
