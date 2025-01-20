/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 17:06:38 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/18 17:06:43 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_open_quotes(char *str)
{
	int	i;
	int	x;

	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (str[i + 1] == '\0')
				return (-1);
			x = sh_jump_to(str + i, str[i]);
			if (str[x + i - 1] != str[i])
				return (-1);
			else
				i += x;
		}
		else
			i++;
	}
	return (0);
}
