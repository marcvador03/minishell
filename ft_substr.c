/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:48:31 by mfleury           #+#    #+#             */
/*   Updated: 2024/06/28 00:44:24 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*str;

	if (s == 0)
		return ((void *)0);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (0);
	i = start;
	while (i < (start + (unsigned int)len) && *s != '\0')
	{
		str[i - start] = s[i];
		i++;
	}
	return (str);
}
