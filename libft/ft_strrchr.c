/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 21:50:14 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/04 15:01:25 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int				i;
	unsigned char	x;

	i = 0;
	x = (unsigned char)c;
	while (s[i] != '\0')
		i++;
	if (s[i] == x)
		return (&((char *)s)[i]);
	i--;
	while (i >= 0)
	{
		if (s[i] == x)
			return (&((char *)s)[i]);
		i--;
	}
	return (0);
}
