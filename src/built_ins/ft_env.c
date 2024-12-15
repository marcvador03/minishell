/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:52:16 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/15 12:21:28 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **args, char ***env)
{
	if (args[1])
	{
		if (ft_strncmp(args[1], "env", 3) != 0)
		{
			printf("'%s': No such file or directory\n", args[1]);
			return (0);
		}
	}
	while (**env)
	{	
		printf("%s\n", **env);
		env++;
	}
	return (0);
}
