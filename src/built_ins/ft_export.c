/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:54:49 by pmorello          #+#    #+#             */
/*   Updated: 2024/11/21 10:21:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

int ft_export(char **envp) 
{
    char **env;

	env = envp;
    while (*env) 
	{
        if (*env && **env != '\0') 
		{
            printf("%s\n", *env);
        }
        env++;
    }
    return (0);
}
