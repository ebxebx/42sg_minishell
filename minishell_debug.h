/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_debug.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 12:34:34 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/02 12:34:49 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEBUG_H
# define MINISHELL_DEBUG_H

# include "minishell.h"

# ifndef DEBUG
#  define DEBUG 0
# endif

void	debug_printf(const char *format_str, ...);

#endif
