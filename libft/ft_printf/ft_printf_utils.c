/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 16:10:14 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/25 16:26:41 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <limits.h>
#include <unistd.h>

int	ft_specs_search(const char *s)
{
	int	i;
	int	j;

	i = 1;
	while (s[i] != '\0')
	{
		j = 0;
		while (j < 9)
			if (s[i] == g_specs[j++])
				return (i);
		i++;
	}
	return (0);
}

void	ft_u_dectobase(char *str, t_ull n, t_uint pos, t_uint base)
{
	char			c;

	if (base >= 35)
		return ;
	if ((n / base) > 0)
		ft_u_dectobase(str, (n / base), (pos - 1), base);
	if ((n % base) >= 10 && (n % base) <= base)
		c = n % base + 87;
	else
	{
		c = n % base + 48;
	}
	str[pos] = c;
}

int	ft_u_basecount(t_ull n, int cnt, t_uint base)
{
	if (base >= 35)
		return (0);
	if ((n / base) > 0)
		cnt += ft_u_basecount((n / base), cnt, base);
	cnt++;
	return (cnt);
}

int	ft_basecount(int n, int cnt, int base)
{
	int	sign;

	if (base >= 35)
		return (0);
	sign = 0;
	if (n == 0)
		return (++cnt);
	if (n < 0)
	{
		sign = TRUE;
		if (n == INT_MIN)
		{
			n = n / 10;
			sign += 1;
		}
		n = -n;
	}
	if ((n / base) > 0)
		cnt += ft_basecount((n / base), cnt, base);
	cnt++;
	return (cnt + sign);
}
