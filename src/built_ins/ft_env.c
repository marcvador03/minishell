/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:52:16 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/10 16:10:35 by pmorello         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

int	ft_env(char **args)
{
	char	**env;

	if (args[1])
	{
		if (ft_strncmp(args[1], "env", 3) != 0)
		{
			printf("'%s': No such file or directory\n", args[1]);
			return (0);
		}
	}
	env = environ;
	while (*env)
	{	
		printf("%s\n", *env);
		env++;
	}
	return (0);
}
