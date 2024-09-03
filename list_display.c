/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_display.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 10:54:12 by mfleury           #+#    #+#             */
/*   Updated: 2024/09/03 17:07:26 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	list_simple_display(t_stack *head_a, t_stack *head_b)
{
	t_stack	*tmp;
	t_stack	*tmp2;

	tmp = head_a;
	tmp2 = head_b;
	ft_printf("a\tb\n");
	while (tmp != NULL || tmp2 != NULL)
	{
		if (tmp == NULL)
		{
			ft_printf(" \t%d\n", tmp2->value);
			tmp2 = tmp2->next;
		}
		else if (tmp2 == NULL)
		{
			ft_printf("%d\t \n", tmp->value);
			tmp = tmp->next;
		}
		else
		{
			ft_printf("%d\t%d\n", tmp->value, tmp2->value);
			tmp = tmp->next;
			tmp2 = tmp2->next;
		}
	}
}
