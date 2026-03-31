/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:35:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/31 00:38:19 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../minishell.h"
# include "../env/env.h"

int		is_builtin(char *builtin_cmd);
int		exec_builtin(t_shell *shell, char **argv);

// Builtins
int		builtin_echo(char **argv);
int		builtin_pwd(t_shell *shell);
int		builtin_env(t_shell	*shell, char **argv);
int		builtin_export(t_shell *shell, char **argv);
//int		builtin_unset(t_shell *shell, char **argv);
//int		unset_arg(char ***env, char *str);

#endif