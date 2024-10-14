/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 21:47:07 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/08 09:28:18 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_l;
	size_t	src_l;

	if (dst == NULL)
		return (0);
	dst_l = ft_strlen(dst);
	if (src == NULL)
		return (dst_l);
	src_l = ft_strlen(src);
	if (size <= dst_l)
		return (src_l + size);
	i = dst_l;
	while (src[i - dst_l] != '\0' && (i + 1) < size)
	{
		dst[i] = src[i - dst_l];
		i++;
	}
	dst[i] = '\0';
	return (dst_l + src_l);
}
