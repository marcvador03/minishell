/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 21:21:35 by mfleury           #+#    #+#             */
/*   Updated: 2024/06/29 21:24:57 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	res;

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
