/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithms_utils.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 23:59:11 by mfleury           #+#    #+#             */
/*   Updated: 2024/09/18 23:54:07 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALGORITHMS_UTILS_H
# define ALGORITHMS_UTILS_H
# include "push_swap.h"

int		search_pos(t_stack *target, t_stack *stk);
t_stack	*search_next_big(unsigned int n, unsigned int min, t_stack *stk);
t_stack	*search_next_small(unsigned int n, unsigned int max, t_stack *stk);
t_stack	*search_value(int n, t_stack *stk);
void	minimize_rotation(t_stack **stk, t_stack *target, char *prt);
void	min_multiple_rotation(t_stack **a, t_stack **b, t_stack *target[2]);
void	update_cost_min(t_stack *a, t_stack *b);
void	update_cost_max(t_stack *b, t_stack *a);
#endif
