/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 21:49:55 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/03 10:56:57 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		j;

	if (ft_strlen(little) == 0)
		return ((char *)big);
	i = 0;
	while (i < len && *big != '\0')
	{
		if (*little == *big)
		{
			j = 0;
			while (little[j] == big[j] && (i + j++) < len)
				if (little[j] == '\0')
					return ((char *)big);
		}
		big++;
		i++;
	}
	return (0);
}
