/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_debug.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:52:19 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/02 12:52:29 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_debug.h"
#include "stdarg.h"

void	debug_printf(const char *format_str, ...)
{
	va_list	args;

	if (!DEBUG)
		return ;
	va_start(args, format_str);
	ft_vprintf(format_str, &args);
	va_end(args);
}
