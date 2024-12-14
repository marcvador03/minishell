/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:23:34 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/14 10:08:01 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_unset(char **args, char **env) 
{
    if (args[1] == NULL)
	{
        printf("Error: Se debe proporcionar el nombre de una variable a eliminar\n");
        return 1; // Error: falta argumento
    }
	while (args[i] != NULL)
	{
		if (unsetenv(args[i]) != 0) 
		{
  		  	perror("Error al eliminar la variable de entorno");
    		return (1);
    	}
	}
    return (0);
}
