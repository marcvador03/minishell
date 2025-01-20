/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:38:02 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/20 00:54:12 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	one_of_char(char c, char *letters)
{
	char	**dict;
	int		i;

	if (letters == NULL)
		return (0);
	dict = ft_split(letters, ',');
	if (dict == NULL)
		return (flush_errors("", 202), 0);
	i = 0;
	while (dict[i] != NULL)
	{
		if (dict[i][0] == c)
			return (free_d(dict), 1);
		i++;
	}
	return (free_d(dict), 0);
}

int	all_of_char(char c, char *letters)
{
	char	**dict;
	int		i;

	if (letters == NULL)
		return (0);
	dict = ft_split(letters, ',');
	if (dict == NULL)
		return (flush_errors("", 202), 0);
	i = 0;
	while (dict[i] != NULL)
	{
		if (dict[i][0] != c)
			return (free_d(dict), 0);
		i++;
	}
	return (free_d(dict), 1);
}

int	none_of_char(char c, char *letters)
{
	char	**dict;
	int		i;

	if (letters == NULL)
		return (0);
	dict = ft_split(letters, ',');
	if (dict == NULL)
		return (flush_errors("", 202), 0);
	i = 0;
	while (dict[i] != NULL)
	{
		if (dict[i][0] == c)
			return (free_d(dict), 0);
		i++;
	}
	return (free_d(dict), 1);
}
