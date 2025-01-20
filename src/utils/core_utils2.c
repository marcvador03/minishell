/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:38:02 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/20 13:09:32 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	one_of_char(char c, char *letters)
{
	char	**dict;
	int		i;

	if (letters == NULL)
		return (FALSE);
	dict = ft_split(letters, ',');
	if (dict == NULL)
		return (flush_errors("", 202), FALSE);
	i = 0;
	while (dict[i] != NULL)
	{
		if (dict[i][0] == c)
			return (free_d(dict), TRUE);
		i++;
	}
	return (free_d(dict), FALSE);
}

int	all_of_char(char c, char *letters)
{
	char	**dict;
	int		i;

	if (letters == NULL)
		return (FALSE);
	dict = ft_split(letters, ',');
	if (dict == NULL)
		return (flush_errors("", 202), FALSE);
	i = 0;
	while (dict[i] != NULL)
	{
		if (dict[i][0] != c)
			return (free_d(dict), FALSE);
		i++;
	}
	return (free_d(dict), TRUE);
}

int	none_of_char(char c, char *letters)
{
	char	**dict;
	int		i;

	if (letters == NULL)
		return (FALSE);
	dict = ft_split(letters, ',');
	if (dict == NULL)
		return (flush_errors("", 202), FALSE);
	i = 0;
	while (dict[i] != NULL)
	{
		if (dict[i][0] == c)
			return (free_d(dict), FALSE);
		i++;
	}
	return (free_d(dict), TRUE);
}

void	init_parse(t_parse *q)
{
	q->i = 0;
	q->j = 0;
	q->k = 0;
	q->prev_pos = 0;
	q->beg_sep = 0;
	q->len = 0;
	q->flag_jump = 0;
}
