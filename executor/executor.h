/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:34:21 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/30 19:00:03 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../minishell.h"
# include "../parsing/ast.h"
# include <fcntl.h>

int	execute_ast(t_shell *shell, t_ast *ast);
int	execute_command_node(t_shell *shell, t_ast *cmd);
int	execute_pipeline(t_shell *shell, t_ast *ast);

#endif
