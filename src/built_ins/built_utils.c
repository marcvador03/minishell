/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:05:20 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/29 14:09:12 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_option(char *str, char *cmd)
{
	if (str == NULL)
		return (0);
	if (str[0] == '-')
	{
		printf("minishell: %s: '%s': invalid option\n", cmd, str);
		return (2);
	}
	return (0);
}

int	check_export_var(char *str)
{
	int	i;

	if (ft_isdigit(str[0]) == 1 || str[0] == '=' || str[0] == '\0')
	{
		printf("minishell: export: '%s': not a valid identifier\n", str);
		return (1);
	}
	i = 0;
	while (str[i] != '\0' && str[i] != '=')
	{
		if (ft_isalnum(str[i]) != 1 && str[i] != '_')
		{
			printf("minishell: export: '%s': not a valid identifier\n", str);
			return (1);
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

int	check_cd_option(char *str)
{
	if (str == NULL)
		return (0);
	if (str[0] == '-' && str[1] != '\0')
	{
		printf("minishell: %s: '%s': invalid option\n", "cd", str);
		return (2);
	}
	return (0);
}

int	chdir_error(char *new_path, char *cur_path, char *arg)
{
	int	err;

	err = errno;
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(err), STDERR_FILENO);
	free_s(new_path);
	free_s(cur_path);
	return (1);
}
