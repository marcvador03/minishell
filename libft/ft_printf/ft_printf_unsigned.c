/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_unsigned.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 16:29:54 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/25 16:26:16 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#define BASE 10

char	*ft_printf_u(unsigned int n)
{
	char	*str;
	int		len;

	len = ft_u_basecount(n, 0, BASE);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[len] = '\0';
	ft_u_dectobase(str, n, len - 1, BASE);
	return (str);
}
