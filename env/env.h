/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:35:04 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/31 14:57:46 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "../minishell.h"

int		env_count(char **env);
int		key_len(char *str);
int		match_key(char *env_entry, char *key);
int		env_index(char **env, char *key);
int		has_equal(char *str);
int		is_valid_identifier(char *str);
char	**dup_env(char **env);

char	**add_env_entry(char **env, char *new_entry);
char	**replace_env_entry(char **env, int index, char *new_entry);
char	**update_or_add(char **env, char *str);
char	**remove_env_entry(char **env, char *str);
int		mark_exported(t_shell *shell, char *str);
int		assign_export(t_shell *shell, char *str);

#endif