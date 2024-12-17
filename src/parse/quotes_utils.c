/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 01:27:07 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/18 00:09:50 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_quote_sections(char *s)
{
	int	n;
	int	i;
	int	pos;

	n = 0;
	i = 0;
	pos = 0;
	while (s[i] != ' ' && s[i] != '\0')
	{
		if (s[i] == 34 || s[i] == 39)
		{
			if (i != pos)
				n++;
			i += sh_jump_to(s + i, s[i]) - 1;
			n++;
			pos = i;
			if (s[i] == ' ' || s[i] == '\0')
				break ;
		}
		i++;
	}
	if (i != pos)
		n++;
	return (n);
}

int	check_open_quotes(char *str)
{
	int	i;
	int	x;

	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == 34 || str[i] == 39)
		{
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
