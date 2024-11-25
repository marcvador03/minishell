/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_quotes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:44:06 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/25 14:35:39 by mfleury          ###   ########.fr       */
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
