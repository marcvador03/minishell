/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 16:30:28 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/02 18:28:10 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <limits.h>

void	ft_putnbr_fd(int n, int fd)
{
	char	x;

	x = -1;
	if (n < 0)
	{
		write (fd, "-", 1);
		if (n == INT_MIN)
		{
			x = (-1) * (n % (-10)) + 48;
			n = n / 10;
		}
		n = -n;
	}
	if (n > 9)
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putchar_fd((char)(n % 10) + 48, fd);
	}
	else
		ft_putchar_fd((char)n + 48, fd);
	if (x >= 0)
		ft_putchar_fd(x, fd);
}
