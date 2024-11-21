/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:20:49 by pmorello          #+#    #+#             */
/*   Updated: 2024/11/21 09:32:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *handle_absolute_path(const char *path) 
{
    char *absolute_path;
	
	absolute_path = ft_strdup(path);

    if (!absolute_path) 
	        perror("Error al asignar memoria para la ruta absoluta");
    return (absolute_path);
}

char *handle_relative_path(const char *path) 
{
    char current_path[1024];
    char *relative_path;

    if (getcwd(current_path, sizeof(current_path)) == NULL) 
	{
        perror("Error al obtener el directorio actual");
        return NULL;
    }
    relative_path = ft_strjoin(current_path, path);
    if (!relative_path)
        perror("Error al asignar memoria para la ruta relativa");
    return (relative_path);
}

int ft_cd(char **args) 
{
    char *path;
    char *newPath;

    if (args[1] != NULL && args[2] != NULL) 
	{
        printf("Error: Demasiados argumentos\n");
        return E2BIG;
    }
    if (args[1] == NULL) 
	{
        path = getenv("HOME");
        if (!path) 
		{
            printf("Error: HOME no está configurado\n");
            return ENOENT;
        }
    } 
	else
		path = args[1];
    if (path[0] == '/') 
        newPath = handle_absolute_path(path);
	else
        newPath = handle_relative_path(path);
    if (!newPath)
        return ENOMEM;
    if (chdir(newPath) != 0) 
	{
        perror("Error al cambiar de directorio");
        free(newPath);
        return errno;
    }
    free(newPath);
    return 0;
}
