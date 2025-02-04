/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 22:52:04 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/04 16:54:58 by mfleury          ###   ########.fr       */
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
	q->x = 0;
	q->prev_pos = 0;
	q->prev_pos2 = 0;
	q->beg_sep = 0;
	q->len = 0;
	q->flag_jump = 0;
	q->flag_sep = 0;
	q->status = 0;
	q->tk = 0;
}

int	max(int n1, int n2)
{
	if (n1 > n2)
		return (n1);
	else
		return (n2);
}

void	swap(char **str1, char **str2)
{
	char	*tmp;

	tmp = *str1;
	*str1 = *str2;
	*str2 = tmp;
}
