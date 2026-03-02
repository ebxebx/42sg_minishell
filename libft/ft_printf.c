/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:13:15 by zchoo             #+#    #+#             */
/*   Updated: 2026/01/17 18:05:22 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
• %c Prints a single character.
• %s Prints a string (as defined by the common C convention).
• %p The void * pointer argument has to be printed in hexadecimal format.
• %d Prints a decimal (base 10) number.
• %i Prints an integer in base 10.
• %u Prints an unsigned decimal (base 10) number.
• %x Prints a number in hexadecimal (base 16) lowercase format.
• %X Prints a number in hexadecimal (base 16) uppercase format.
• %% Prints a percent sign.
*/

int	handle_text(va_list *ap, const char flag)
{
	size_t	cnt;
	char	*s;

	cnt = 0;
	if (flag == 'c')
	{
		ft_putchar_fd((char)va_arg(*ap, int), 1);
		cnt++;
	}
	else if (flag == 's')
	{
		s = va_arg(*ap, char *);
		if (!s)
			s = "(null)";
		ft_putstr_fd(s, 1);
		cnt += ft_strlen(s);
	}
	return ((int)cnt);
}

int	handle_pointer(va_list *ap, const char flag)
{
	size_t			cnt;
	void			*ptr;
	char			*addr_str;
	const char		*prefix;

	cnt = 0;
	if (flag == 'p')
	{
		prefix = "0x";
		ptr = va_arg(*ap, void *);
		if (!ptr)
		{
			ft_putstr_fd("(nil)", 1);
			return (5);
		}
		addr_str = ft_put_addr(ptr, (char *)prefix, "");
		if (!addr_str)
			return (0);
		ft_putstr_fd(addr_str, 1);
		cnt += ft_strlen(addr_str);
		free(addr_str);
	}
	return (cnt);
}

int	handle_flag(va_list *ap, const char flag)
{
	size_t	cnt;

	cnt = 0;
	if (flag == 'c' || flag == 's')
		cnt += handle_text(ap, flag);
	else if (flag == 'p')
		cnt += handle_pointer(ap, flag);
	else if (ft_strchr("dfiuxX", flag))
		cnt += handle_number(ap, flag);
	else if (flag == '%')
	{
		ft_putchar_fd(flag, 1);
		cnt++;
	}
	return (cnt);
}

int	ft_printf(const char *fmt, ...)
{
	va_list	ap;
	size_t	cnt;

	cnt = 0;
	va_start(ap, fmt);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			cnt += handle_flag(&ap, *fmt);
		}
		else
		{
			ft_putchar_fd(*fmt, 1);
			cnt++;
		}
		fmt++;
	}
	va_end(ap);
	return (cnt);
}

/* int main(void)
{
	ft_printf("", 1, 2, 3);
	return (0);
} */