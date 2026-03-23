/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:40 by ka-tan            #+#    #+#             */
/*   Updated: 2026/03/23 19:45:26 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>
#include "ft_printf/include/ft_printf.h"

int	builtin_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	(void)shell;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_printf("%s\n", cwd);
	else
	{
		perror("pwd");
		return (1);
	}
	return (0);
}

int	main(void)
{
	t_shell	shell;
	int		status;

	status = builtin_pwd(&shell);
	printf("return value = %d\n", status);
	return (0);
}
