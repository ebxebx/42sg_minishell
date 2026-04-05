/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:34:21 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/04 15:31:00 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../minishell.h"
# include "../parsing/ast.h"
# include <fcntl.h>

# define HERE_DOC_TMP ".abc_here_doc_xyz"

int		execute_ast(t_shell *shell, t_ast *ast);
void	execute_ast_child(t_shell *shell, t_ast *ast);
int		execute_command_node(t_shell *shell, t_ast *cmd);
int		execute_pipeline(t_shell *shell, t_ast *ast);
void	execute_command_child(t_shell *shell, t_ast *cmd);
int		apply_redirections(t_ast *cmd);

// utils...
void	close_all_fds(void);
char	*strip_quotes(const char *value);
char	**build_argv(char *cmdline);

#endif
