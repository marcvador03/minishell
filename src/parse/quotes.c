/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:44:06 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/28 16:06:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	check_open_q(char *str)
{
	int		i;
	int		cnt[40];
	char	c;

	i = 0;
	cnt[39] = 0;
	cnt[34] = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 39 || str[i] == 34)
		{
			c = str[i];
			cnt[(int)c]++;
			i++;
			while (str[i] != '\0')
			{
				if (str[i++] == c)
				{
					cnt[(int)c]++;
					break;
				}
			}
			if (str[i] != '\0')
				break;
		}
		i++;
	}
	if (cnt[39] % 2 != 0 ||  cnt[34] % 2 != 0)
		return (-1);
	return (0);
}

int	sh_jump_to(char *str, char c)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 1;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (++i);
		i++;
	}	
	return (0);
}

int	sh_skip(char *str, char c)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i] == c)
		i++;
	return (i);
}
