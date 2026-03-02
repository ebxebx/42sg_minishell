/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zchoo <zchoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:02:24 by zchoo             #+#    #+#             */
/*   Updated: 2026/01/04 19:12:29 by zchoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

int		ft_printf(const char *fmt, ...);
void	*ft_put_hex_str(unsigned long long num, int to_upper);
void	*ft_put_addr(void *addr, char *pad_left, char *post_fix);
void	ft_putnbr2_fd(long nbr_l, int fd);
int		get_nbr_length(long l);

int		handle_flag(va_list *ap, const char flag);
int		handle_text(va_list *ap, const char flag);
int		handle_pointer(va_list *ap, const char flag);
int		handle_hexadecimal(va_list *ap, const char flag);
int		handle_signed_number(va_list *ap, const char flag);
int		handle_unsigned_number(va_list *ap, const char flag);
int		handle_number(va_list *ap, const char flag);

#endif