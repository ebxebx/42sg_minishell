/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:30:09 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/30 03:01:42 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env/env.h"
#include "libft/libft.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->status = 0;
	shell->env = dup_env(env);
	shell->export = dup_env(env);
	if (!shell->env || !shell->export)
		exit(EXIT_FAILURE);
}

void	free_shell(t_shell *shell)
{
	ft_strarr_free(shell->env);
}
