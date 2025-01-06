/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 18:06:03 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/06 18:10:08 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ll	ll_atoi(const char *nptr)
{
	int		sign;
	t_ll	res;

	if (*nptr == '\0')
		return (0);
	res = 0;
	sign = 1;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == 45 || *nptr == 43)
		if (*nptr++ == 45)
			sign *= -1;
	if (*nptr != '\0' && *nptr >= 48 && *nptr <= 57)
		res = res + (*nptr++ - 48);
	while (*nptr != '\0' && *nptr >= 48 && *nptr <= 57)
	{
		if (res != 0)
			res *= 10;
		res = res + (*nptr++ - 48);
	}
	return (res * sign);
}

static unsigned int	ft_itoa_len(t_ll n)
{
	unsigned int	cnt;

	cnt = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / 10;
		cnt++;
	}
	return (cnt);
}

static unsigned int	ft_itoa_lim(int *temp, t_ll *n)
{
	int	sign;

	sign = 0;
	if (*n < 0)
	{
		sign = 1;
		if (*n == LLONG_MIN)
		{
			*temp = *n % (-10);
			*temp = -(1) * *temp;
			*n = *n / 10;
			sign += 1;
		}
		*n = -(1) * *n;
	}
	return (sign);
}

char	*ll_itoa(t_ll n)
{
	unsigned int	cnt;
	unsigned int	sign;
	char			*str;
	int				temp;

	temp = -1;
	sign = ft_itoa_lim(&temp, &n);
	cnt = ft_itoa_len(n) + sign;
	str = (char *)malloc((cnt + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[cnt--] = '\0';
	if (n == 0)
		str[0] = 48;
	if (temp >= 0)
		str[cnt--] = temp + 48;
	while (n != 0)
	{
		str[cnt--] = (n % 10) + 48;
		n = n / 10;
	}
	if (sign >= 1)
		str[0] = '-';
	return (str);
}
