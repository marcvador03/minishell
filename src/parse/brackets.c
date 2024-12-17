/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brackets.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 10:02:58 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/17 10:04:40 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_brackets(char *line, int i)
{
	while (line[i] == 34 || line[i] == 39)
		i += sh_jump_to(line + i, line[i]);
	if (line[i] == '\0')
		return (-1);
	i += sh_skip(line + i + 1, ' ');
	if (line[i] == '\0')
		return (-1);
	if (line[i + 1] == '\0' || line[i + 1] == ')')
		return (-1);
	return (0);
}

int	count_brackets(t_shell *sh, char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		while (line[i] == 34 || line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '\0')
			return (0);
		if (line[i] == '(')
		{
			if (check_brackets(line, i) == -1)
				return (-1);
			sh->bracket[0]++;
			ft_memset(line + i, ' ', 1);
		}
		else if (line[i] == ')')
		{
			sh->bracket[1]++;
			ft_memset(line + i, ' ', 1);
		}
		i++;
	}
	return (0);
}
