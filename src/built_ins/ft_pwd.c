/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:44:34 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/14 10:13:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **args, char **env)
{
	char *buffer;
	size_t	size;
	
	if (env == NULL)
		return (-1);
	if (args[1] != NULL)
		return (E2BIG);
	size = 1024;
	buffer = (char *)ft_calloc(sizeof(char), size);
	if (buffer == NULL)
		return (ENOMEM);
	getcwd(buffer, size);
	if (buffer == NULL)
		return (free(buffer), errno);
	printf("%s\n", buffer);
	return (free(buffer), 0);
}
