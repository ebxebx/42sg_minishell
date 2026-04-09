/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:35:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/09 13:03:17 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../env/env.h"
# include "../expander/expand.h"
# include "../libft/libft.h"
# include "../minishell.h"
# include <sys/stat.h>

// Builtins
int		builtin_echo(char **argv);
int		builtin_pwd(t_shell *shell);
int		builtin_env(t_shell *shell, char **argv);
int		builtin_export(t_shell *shell, char **argv);
int		builtin_unset(t_shell *shell, char **argv);
int		builtin_cd(t_shell *shell, char **argv);
int		builtin_exit(t_shell *shell, char **argv);

// utils
int		ft_atoll_exit(char *str, long long *out);
int		print_export(char **export);
void	sort_export(char **arg);

#endif
