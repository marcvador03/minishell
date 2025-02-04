/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:16:56 by pmorello          #+#    #+#             */
/*   Updated: 2025/02/04 14:32:48 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_params(char **args)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] != NULL)
	{
		if (*args[i] == '-')
		{
			j = 1;
			if (args[i][j] == '\0')
				return (i);
			while (args[i][j] != '\0')
			{
				if (args[i][j] != 'n')
					return (i);
				j++;
			}
		}
		else
			return (i);
		i++;
	}
	return (i);
}

int	check_spaces(char **args, int i)
{
	int	j;

	if (args[i + 1] == NULL)
		return (1);
	j = 0;
	while (args[i][j] != '\0')
	{
		if (args[i][j++] != ' ')
			return (0);
	}
	return (0);
}

int	ft_echo(char **args, t_env *env)
{
	int	i;
	int	new_line;

	if (env == NULL)
		return (-1);
	new_line = check_params(args);
	i = new_line;
	while (args[i] != NULL)
	{
		printf("%s", args[i]);
		if (check_spaces(args, i) == 0)
			printf(" ");
		i++;
	}
	if (new_line == 1)
		printf("\n");
	return (0);
}
