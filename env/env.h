/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:35:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/30 12:54:14 by ka-tan           ###   ########.fr       */
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
# include "../libft/libft.h"
# include <linux/limits.h>
# include "../minishell.h"

int		is_builtin(char *builtin_cmd);
int		exec_builtin(t_shell *shell, char **argv);

int		env_count(char **env);
int		key_len(char *str);
int		match_key(char *env_entry, char *key);
int		env_index(char **env, char *key);
int		has_equal(char *str);
int		is_valid_identifier(char *str);
char	**dup_env(char **env);

char	**add_env_entry(char ***env, char *new_entry);
char	**replace_env_entrya(char **env, int index, char *new_entry);
char	**update_or_add(char **env, char *str);
int		mark_exported(t_shell *shell, char *str);
int		assign_export(t_shell *shell, char *str);

void	remove_env_entry(char **env, int index);

#endif