/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:44:06 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/25 20:47:18 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_open_q(char *str)
{
	int	i;
	int	cnt_s;
	int	cnt_d;

	i = 0;
	cnt_s = 0;
	cnt_d = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 39)
			cnt_s++;
		if (str[i] == 34)
			cnt_d++;
		i++;
	}
	if (cnt_s % 2 != 0 ||  cnt_d % 2 != 0)
		return (-1);
	return (0);

}

int	skip_quotes(char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 1;
	while (str[i] != '\0')
	{
		if (str[i] == 34 || str[i] == 39)
			return (++i);
		i++;
	}	
	return (0);
}

int	skip_spaces(char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i] == ' ')
		i++;
	return (i);
}
