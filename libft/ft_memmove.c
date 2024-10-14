/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 21:42:28 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/03 10:03:45 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*src_t;
	char	*dest_t;

	src_t = (char *)src;
	dest_t = (char *)dest;
	if (dest > src)
	{
		i = n;
		while (i-- > 0)
			dest_t[i] = src_t[i];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			dest_t[i] = src_t[i];
			i++;
		}
	}
	return (dest);
}
