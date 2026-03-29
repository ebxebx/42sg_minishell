/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_helper2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 14:00:11 by zchoo             #+#    #+#             */
/*   Updated: 2026/03/29 19:16:00 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_hexadecimal(va_list *ap, const char flag)
{
	size_t			cnt;
	unsigned int	num;
	char			*hex_str;
	int				to_upper;

	cnt = 0;
	to_upper = 0;
	if (flag == 'X')
		to_upper = 1;
	num = va_arg(*ap, unsigned int);
	if (num == 0)
	{
		ft_putchar_fd('0', 1);
		return (1);
	}
	hex_str = ft_put_hex_str((unsigned long long)num, to_upper);
	if (!hex_str)
		return (0);
	ft_putstr_fd(hex_str, 1);
	cnt += ft_strlen(hex_str);
	free(hex_str);
	return (cnt);
}

int	handle_signed_number(va_list *ap, const char flag)
{
	size_t	cnt;
	int		d;

	cnt = 0;
	if (flag == 'd' || flag == 'i')
	{
		d = va_arg(*ap, int);
		ft_putnbr_fd(d, 1);
		cnt += get_nbr_length(d);
	}
	return (cnt);
}

int	handle_unsigned_number(va_list *ap, const char flag)
{
	size_t			cnt;
	unsigned int	u;

	cnt = 0;
	if (flag == 'u')
	{
		u = va_arg(*ap, unsigned int);
		ft_putnbr2_fd(u, 1);
		cnt += get_nbr_length(u);
	}
	return (cnt);
}

int	handle_number(va_list *ap, const char flag)
{
	size_t	cnt;

	cnt = 0;
	if (flag == 'd' || flag == 'i')
		cnt += handle_signed_number(ap, flag);
	else if (flag == 'u')
		cnt += handle_unsigned_number(ap, flag);
	else if (flag == 'x' || flag == 'X')
		cnt += handle_hexadecimal(ap, flag);
	else if (flag == 'f')
		cnt += handle_floating_point_number(ap, flag);
	return (cnt);
}
