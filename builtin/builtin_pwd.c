/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:40 by ka-tan            #+#    #+#             */
/*   Updated: 2026/03/30 18:10:51 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_pwd(t_shell *shell)
{
	char	cwd;

	(void)shell;
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
		ft_putendl_fd(cwd, 1);
	else
	{
		perror("pwd");
		return (1);
	}
	free(cwd);
	return (0);
}

// int	main(void)
// {
// 	t_shell	shell;
// 	int		status;

// 	status = builtin_pwd(&shell);
// 	ft_printf("return value = %d\n", status);
// 	return (0);
// }
