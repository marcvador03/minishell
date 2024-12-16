/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:16:56 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/16 14:29:22 by mfleury          ###   ########.fr       */
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

int	ft_echo(char **args, char ***env)
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
		if (args[++i] != NULL)
			printf(" ");
	}
	if (new_line == 1)
		printf("\n");
	return (0);
}
