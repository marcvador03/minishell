/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:41:31 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/12 23:28:10 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sh_strpos(const char *big, const char *little)
{
	size_t	i;
	int		j;
	char	*str;

	str = (char *)big;
	if (ft_strlen(little) == 0)
		return (ft_strlen(big) + 1);
	i = 0;
	while (*str != '\0')
	{
		if (*str == 34 || *str == 39)
			i += sh_jump_to2(&str, *str);
		if (*little == *str)
		{
			j = 0;
			while (little[j] == str[j])
				if (little[++j] == '\0')
					return (i);
		}
		else if (*str == '\0')
			return (ft_strlen(big) + 1);
		str++;
		i++;
	}
	return (ft_strlen(big) + 1);
}
