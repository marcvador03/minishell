/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:52:16 by pmorello          #+#    #+#             */
/*   Updated: 2025/02/05 21:44:49 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *str, char *cmd);

int	ft_env(char **args, t_env **env)
{
	int	err;

	err = 0;
	if (args[0] == NULL)
		return (207);
	err = check_option(args[1], "env");
	if (err == 2)
		return (err);
	while ((*env) != NULL)
	{
		if ((*env)->value != NULL)
			printf("%s%c%s\n", (*env)->varname, '=', (*env)->value);
		*env = (*env)->next;
	}
	return (err);
}
