/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:56:07 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/21 10:05:20 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sh_trim_spaces(char *str)
{
	char	*new_str;

	if (str == NULL)
		return (NULL);
	new_str = ft_strtrim(str, " ");
	if (new_str == NULL)
		return (flush_errors("", 202, ""), str);
	free_s(str);
	return (new_str);
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
	return (i);
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
	if (ft_strncmp(str, "", max(ft_strlen(str), 2)) == 0)
		return (-1);
	return (0);
}

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
		while (*str == 34 || *str == 39)
			i += sh_jump_to2(&str, *str);
		if (*little == *str)
		{
			j = 0;
			while (little[j] == str[j])
				if (little[++j] == '\0')
					return (i);
		}
		else if (*str == '\0')
			return (ft_strlen(big));
		str++;
		i++;
	}
	return (ft_strlen(big));
}
