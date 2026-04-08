/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:34:21 by zchoo             #+#    #+#             */
/*   Updated: 2026/04/08 15:28:40 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "../builtin/builtin.h"
# include "../minishell.h"
# include "../parsing/ast.h"
# include "../parsing/minishell_tokenize.h"
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>

# define HERE_DOC_TMP ".abc_here_doc_xyz"

/* For pipeline execution */
enum	e_pipe_side
{
	LEFT,
	RIGHT
};

int		execute_ast(t_shell *shell, t_ast *ast);
void	execute_ast_child(t_shell *shell, t_ast *ast);
int		execute_command_node(t_shell *shell, t_ast *cmd);
int		execute_pipeline(t_shell *shell, t_ast *ast);
void	execute_command_child(t_shell *shell, t_ast *cmd);
int		apply_redirections(t_ast *cmd);

// heredoc related
int		run_heredoc_child(t_shell *shell, char *limiter, char *path,
			int should_expand);
int		preprocess_heredocs(t_ast *ast, t_shell *shell);
int		is_heredoc_tmp_file(const char *path);
char	*make_heredoc_tmp_path(void);
char	**collect_heredoc_lines(char *limiter, int *out_count);
void	free_lines(char **lines, int count);
void	cleanup_heredoc_tmps(t_ast *ast);
int		read_heredoc_to_path(t_shell *shell, char *limiter, char *path,
			int should_expand);

// execve related
int		exec_with_path(char **argv, char **env);

// run builtin
int		is_builtin_command(char *cmd);
int		run_builtin(t_shell *shell, char **argv);
int		is_parent_builtin(char *cmd);
int		execute_parent_builtin(t_shell *shell, char **argv);

// utils...
char	**get_paths_from_env(char **env);
char	*get_full_path(char *cmd, char *path);
void	close_all_fds(void);
char	*strip_quotes(const char *value);
// char	**build_argv(t_shell *shell, char *cmdline);

#endif
