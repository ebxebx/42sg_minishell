/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ka-tan <ka-tan@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 16:06:37 by ka-tan            #+#    #+#             */
/*   Updated: 2026/04/04 20:41:25 by ka-tan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <limits.h>

// +ve no. limit is LLong_max, -ve is LLong_min
static int	ft_extract_exit_num(char *str, long long *out, int sign)
{
	unsigned long long	num;
	unsigned long long	limit;
	int					digit;

	num = 0;
	if (sign == -1)
		limit = (unsigned long long)LLONG_MAX + 1ULL;
	else
		limit = (unsigned long long)LLONG_MAX;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		digit = *str - '0';
		if (num > limit / 10 || (num == limit / 10
				&& (unsigned long long)digit > limit % 10))
			return (0);
		num = num * 10 + digit;
		str++;
	}
	if (sign == -1 && num == (unsigned long long)LLONG_MAX + 1ULL)
		*out = LLONG_MIN;
	else
		*out = (long long)num * sign;
	return (1);
}

int	ft_atoll_exit(char *str, long long *out)
{
	int	sign;

	if (!str || !*str)
		return (0);
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	return (ft_extract_exit_num(str, out, sign));
}
