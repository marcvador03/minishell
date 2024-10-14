/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 10:56:51 by mfleury           #+#    #+#             */
/*   Updated: 2024/08/06 16:22:48 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

void	*ft_printf_c(int n, int *cnt)
{
	ssize_t			byte;
	unsigned char	c;

	c = (unsigned char)n;
	byte = write(1, &c, 1);
	if (byte == -1)
		*cnt = -1;
	else
		*cnt += byte;
	return (NULL);
}
