/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:23:34 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/16 14:31:08 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unset_var(char *str)
{
	if (*ft_strchr(str, '=') != '\0')
		return (-1);
	if (*ft_strchr(str, '\\') != '\0')
		return (-1);
	if (*ft_strchr(str, '$') != '\0')
		return (-1);
	if (*ft_strchr(str, '"') != '\0')
		return (-1);
	if (ft_isdigit(str[0]) == 1 || str[0] == '/' || str[0] == '\0')
		return (-1);
	return (0);
}

int	ft_unset(char **args, char ***env)
{
	int	i;

	i = 1;
	while (args[i] != NULL)
	{
		if (check_unset_var(args[i]) == -1)
			return (11);
		if (sh_getenv(*env, args[i]) != NULL)
			*env = sh_del_env(env, args[i]);
		i++;
	}
	return (0);
}
