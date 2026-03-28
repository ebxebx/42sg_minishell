/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:35:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/28 20:49:02 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

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
# include "../minishell.h"

int		is_builtin(char *builtin_cmd);
int		exec_builtin(t_shell *shell, char **argv);

int		builtin_echo(char **argv);
int		builtin_pwd(t_shell *shell);
int		builtin_env(t_shell	*shell, char **argv);
char	**dup_env(char **envp);
void	free_env(char **envp);

#endif