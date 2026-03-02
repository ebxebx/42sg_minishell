/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:13:15 by zchoo             #+#    #+#             */
/*   Updated: 2026/01/03 13:20:02 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_vprintf(const char *fmt, va_list *args)
{
	size_t	cnt;

	cnt = 0;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			cnt += handle_flag(args, *fmt);
		}
		else
		{
			ft_putchar_fd(*fmt, 1);
			cnt++;
		}
		fmt++;
	}
	return (cnt);
}

/* int main(void)
{
	ft_printf("", 1, 2, 3);
	return (0);
} */