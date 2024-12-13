/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 01:02:14 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/13 12:32:55 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*expand_env(char *line);

int	init_data_brackets(t_shell *tmp, int *a, int *b)
{
	tmp->s_line = expand_env(tmp->s_line);
	if (tmp->s_line == NULL)
		return (-1);
	tmp->bracket[0] += *a;
	tmp->bracket[1] += *a;
	*a = tmp->bracket[0];
	*a = tmp->bracket[1];
	*b = 0;
	return (0);
}

int	check_input(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == 32 || line[i] == 33 || line[i] == 58)
			i++;
		else if (line[i] == 59 || line[i] == 92)
			i++;
		else
			return (0);
	}
	return (-1);
}
