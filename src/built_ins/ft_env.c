/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:52:16 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/08 13:51:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **args, t_env *env)
{
	if (args[0] == NULL)
		return (-1);
	while (env != NULL)
	{
		if (env->value != NULL)
			printf("%s%c%s\n", env->varname, '=', env->value);
		env = env->next;
	}
	return (0);
}
