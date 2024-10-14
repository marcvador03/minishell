/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 00:05:08 by mfleury           #+#    #+#             */
/*   Updated: 2024/08/20 10:50:17 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_t)
{
	t_list	*tmp;

	if (lst != NULL)
	{
		if (*lst == NULL)
			*lst = new_t;
		else
		{
			tmp = ft_lstlast(*lst);
			tmp->next = new_t;
		}
	}
}
