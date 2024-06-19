/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 17:20:52 by mfleury           #+#    #+#             */
/*   Updated: 2024/06/19 17:32:48 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lib_ft.h>

char *ft_strjoin(char const *s1, char const *s2)
{
	int	len;
	int	s1_len;
	char	*str;
	int	i;

	s1_len = ft_strlen(s1);
	len = s1_len + ft_strlen(s2);
	str = (char *)malloc(len * sizeof(char));
	if (str == NULL)
		return (0);
	i = 0;
	while (i < s1_len)
	{
		str[i] = s1[i]; 
		i++;
	}
	while (i < len)
	{
		str[i] = s2[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}
