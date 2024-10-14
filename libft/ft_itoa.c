/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 21:28:32 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/04 15:06:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <limits.h>

static unsigned int	ft_itoa_len(int n)
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

static unsigned int	ft_itoa_lim(int *temp, int *n)
{
	int	sign;

	sign = 0;
	if (*n < 0)
	{
		sign = 1;
		if (*n == INT_MIN)
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

char	*ft_itoa(int n)
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
