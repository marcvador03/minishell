/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:05:20 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/17 15:40:52 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	swap(char **str1, char **str2)
{
	char	*tmp;

	tmp = *str1;
	*str1 = *str2;
	*str2 = tmp;
}

int	export_count(char **env)
{
	int	n;

	n = 0;
	while (env[n] != NULL)
		n++;
	return (n);
}

int	check_export_var(char *str)
{
	int	i;

	if (ft_isdigit(str[0]) == 1 || str[0] == '=' || str[0] == '\0')
	{
		g_status = 10;
		printf("minishell: export: '%s': not a valid identifier\n", str);
		return (-1);
	}
	i = 0;
	while (str[i] != '\0' && str[i] != '=')
	{
		if (ft_isalnum(str[i]) != 1 && str[i] != '_')
		{
			g_status = 1;
			printf("minishell: export: '%s': not a valid identifier\n", str);
			return (-1);
		}
		i++;
	}
	return (0);
}

char	*export_env_value(char *args, int n)
{
	char	*res;

	if (args[n] == '\0')
		res = NULL;
	else
		res = &args[n + 1];
	return (res);
}
