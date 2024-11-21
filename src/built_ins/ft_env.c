/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:52:16 by pmorello          #+#    #+#             */
/*   Updated: 2024/11/21 10:22:07 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

int	ft_env(char **envp)
{
	char	**env;

	env = envp;
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	return (0);
}
