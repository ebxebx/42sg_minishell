/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:35:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/30 12:53:54 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <linux/limits.h>
# include "ft_printf/include/ft_printf.h"
# include "../minishell.h"

int		is_builtin(char *builtin_cmd);
int		exec_builtin(t_shell *shell, char **argv);

// Builtins
int		builtin_echo(char **argv);
int		builtin_pwd(t_shell *shell);
int		builtin_env(t_shell	*shell, char **argv);
int		builtin_export(t_shell *shell, char **argv);
int		export_arg(t_shell *shell, char *str);
int		mark_export(char ***env, char *key);
int		set_env_var(char ***env, char *str);
int		builtin_unset(t_shell *shell, char **argv);
int		unset_arg(char ***env, char *str);


#endif