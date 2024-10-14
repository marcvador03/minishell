/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hexa.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:44:29 by mfleury           #+#    #+#             */
/*   Updated: 2024/07/27 16:00:59 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#define BASE 16

static char	*ft_printf_free(char **str, int broken)
{
	char	*res;
	int		i;

	if (broken == TRUE)
		res = NULL;
	else
		res = ft_strdup(str[0]);
	i = 0;
	while (i <= 2)
		free(str[i++]);
	free(str);
	return (res);
}

static char	*ft_fill_mainstr(int len, t_ull n, char **str)
{
	char	*s;

	s = (char *)malloc((len + 1) * sizeof(char));
	if (s == NULL)
		return (ft_printf_free(str, TRUE));
	ft_u_dectobase(s, n, len - 1, BASE);
	s[len] = '\0';
	return (s);
}

static char	*ft_fill_compstr(char cas, char **str)
{
	char	*s;

	if (cas == 'p')
	{
		s = ft_strdup("0x");
		if (s == NULL)
			return (ft_printf_free(str, TRUE));
	}
	else
		s = NULL;
	return (s);
}

char	*ft_printf_x(t_ull n, char cas)
{
	char	**str;
	int		len;
	int		i;

	if (cas == 'p' && n == 0)
		return (ft_strdup(PTRNUL));
	str = (char **)malloc(3 * sizeof(char *));
	if (str == NULL)
		return (NULL);
	len = ft_u_basecount(n, 0, BASE);
	str[1] = ft_fill_mainstr(len, n, str);
	str[2] = ft_fill_compstr(cas, str);
	if (str[2] != NULL)
		str[0] = ft_strjoin(str[2], str[1]);
	else
		str[0] = ft_strdup(str[1]);
	if (str[0] == NULL)
		return (ft_printf_free(str, TRUE));
	i = -1;
	if (cas == 'X')
		while (str[0][++i] != '\0')
			str[0][i] = ft_toupper(str[0][i]);
	return (ft_printf_free(str, FALSE));
}
