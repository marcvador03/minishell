/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:16:56 by pmorello          #+#    #+#             */
/*   Updated: 2024/11/21 10:03:34 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args)
{
	size_t	len;
	int	i;
	int new_line;

	new_line = 1;
	i = 1;
	len = ft_strlen(args[i]);
	while (args[i] != NULL)
	{
		if (ft_strncmp(args[i], "-n", len) == 0)
			new_line = 0;
		else
		{
			printf("%s", args[i]);
			//if (i < argc - 1)
			if (args[i + 1] != NULL)
				printf (" ");
		}
		i++;
	}
	if (new_line)
		printf("\n");
	return (0);
}
