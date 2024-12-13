/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 01:27:07 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/13 21:19:53 by mfleury          ###   ########.fr       */
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
			i += sh_jump_to(s + i, s[i]);
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
