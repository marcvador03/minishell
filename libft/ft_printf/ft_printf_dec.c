/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_dec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 11:18:13 by mfleury           #+#    #+#             */
/*   Updated: 2024/08/07 01:09:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#define BASE 10

char	*ft_printf_d(int n)
{
	char	*str;

	ft_basecount(n, 0, BASE);
	str = ft_itoa(n);
	return (str);
}
