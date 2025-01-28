/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core2_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 22:52:04 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 18:01:47 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set(int err_code, int *err)
{
	*err = err_code;
}

void	init_parse(t_parse *q)
{
	q->i = 0;
	q->j = 0;
	q->k = 0;
	q->prev_pos = 0;
	q->prev_pos2 = 0;
	q->beg_sep = 0;
	q->len = 0;
	q->flag_jump = 0;
	q->flag_bracket = 0;
	q->status = 0;
	q->tk = 0;
}

int	ft_isalnum_plus(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	else if (c >= 48 && c <= 57)
		return (1);
	else if (c == '@' || c == '_' || c == '?' || c == 34 || c == 39)
		return (1);
	else
		return (0);
}

int	ft_isalnum_plus_q(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	else if (c >= 48 && c <= 57)
		return (1);
	else if (c == '@' || c == '_' || c == '?')
		return (1);
	else
		return (0);
}

int	max(int n1, int n2)
{
	if (n1 > n2)
		return (n1);
	else
		return (n2);
}
