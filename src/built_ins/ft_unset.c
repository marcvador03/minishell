/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:23:34 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/08 00:48:18 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unset_var(char *str)
{
	int	i;

	if (*ft_strchr(str, '=') != '\0')
		return (-1);
	if (*ft_strchr(str, '\\') != '\0')
		return (-1);
	if (*ft_strchr(str, '$') != '\0')
		return (-1);
	if (*ft_strchr(str, '"') != '\0')
		return (-1);
	if (ft_isdigit(str[0]) == 1 || str[0] == '/' || str[0] == '\0')
	{
		if (str[0] != '_')
			return (-1);
	}
	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (-1);
		i++;
	}
	return (0);
}

int	ft_unset(char **args, t_env *env)
{
	int	i;

	i = 1;
	while (args[i] != NULL)
	{
		if (check_unset_var(args[i]) == -1)
		{
			printf("minishell: unset: '%s': not a valid identifier\n", args[i]);
			g_status = 11;
		}
		if (sh_getenv(env, args[i]) != NULL)
			sh_delenv(env, args[i]);
		i++;
	}
	return (0);
}
