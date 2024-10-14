/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:36:03 by pmorello          #+#    #+#             */
/*   Updated: 2024/10/14 11:21:18 by pmorello         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_cd(char *path)
{
	if (path == NULL)
		path = getenv("HOME");
	chdir(path);
}

void	ft_pwd(int argc)
{
	char *buffer;
	size_t	size;
	
	size = 1024;
	buffer = (char *)malloc(sizeof(char) * size);
	getcwd(buffer, size);
	printf("%s\n", buffer);
	free (buffer);
	if (argc > 1)
		printf("pwd : too many arguments");
	
}
