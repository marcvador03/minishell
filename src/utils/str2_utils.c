/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str2_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:38:02 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 16:15:23 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	oneofchar(char c, char *letters)
{
	char	**dict;
	int		i;

	if (letters == NULL)
		return (FALSE);
	dict = ft_split(letters, ',');
	if (dict == NULL)
		return (flush_errors("", 202, 0), FALSE);
	i = 0;
	while (dict[i] != NULL)
	{
		if (dict[i][0] == c)
			return (free_d(dict), TRUE);
		i++;
	}
	return (free_d(dict), FALSE);
}

int	noneofchar(char c, char *letters)
{
	char	**dict;
	int		i;

	if (letters == NULL)
		return (FALSE);
	dict = ft_split(letters, ',');
	if (dict == NULL)
		return (flush_errors("", 202, 0), FALSE);
	i = 0;
	while (dict[i] != NULL)
	{
		if (dict[i][0] == c)
			return (free_d(dict), FALSE);
		i++;
	}
	return (free_d(dict), TRUE);
}

char	*sh_trim_spaces(char *str)
{
	char	*new_str;

	if (str == NULL)
		return (NULL);
	new_str = ft_strtrim(str, " ");
	if (new_str == NULL)
		return (flush_errors("", 202, 0), str);
	free_s(str);
	return (new_str);
}
