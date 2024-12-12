/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:56:07 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/12 01:29:38 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	sh_jump_to2(char **str, char c)
{
	int		i;
	char	*ptr;

	if (*str == NULL)
		return (0);
	i = 1;
	ptr = *str;
	while (ptr[i] != '\0')
	{
		if (ptr[i] == c)
		{
			*str = ptr + ++i;
			return (i);
		}
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

int	sh_check_empty(char *str)
{
	if (str == NULL)
		return (0);
	if (ft_strncmp(str, "", ft_strlen(str)) == 0)
		return (-1);
	return (0);
}
