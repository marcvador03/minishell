/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:44:34 by pmorello          #+#    #+#             */
/*   Updated: 2024/11/21 09:31:08 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **args)
{
	char *buffer;
	size_t	size;
	
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
