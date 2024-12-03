/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:51:35 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/03 18:54:10 by mfleury          ###   ########.fr       */
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
		{
			i += sh_jump_to2(&str, *str);
//			str = str + sh_jump_to(str, *str);
		}
		if (*little == *str)
		{
			j = 0;
			while (little[j] == str[j])
				if (little[++j] == '\0')
					return (i);
		}
		str++;
		i++;
	}
	return (ft_strlen(big) + 1);
}

char	*sh_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		j;

	if (ft_strlen(little) == 0)
		return ((char *)big);
	i = 0;
	while (i < len && *big != '\0')
	{
		if (*big == 34 || *big == 39)
			big = big + sh_jump_to((char *)big, *big);
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

char	*sh_strtrim(char *str, char *set, char offset)
{
	char	*res;

	res = ft_strtrim(str + offset, set);
	free_s(str);
	return (res);
}

void	sh_strtrim2(char **str, char *set, char offset)
{
	char	*tmp;	

	tmp = ft_strdup(*str);
	free(*str);
	*str = ft_strtrim(tmp + offset, set);
	free(tmp);
	return ;
}
