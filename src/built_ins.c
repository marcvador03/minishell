/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:36:03 by pmorello          #+#    #+#             */
/*   Updated: 2024/10/14 13:09:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_cd(char **args)
{
	if (args[2] != NULL)
		printf("pwd : too many arguments");
	if (args[1] == NULL)
		args[1] = getenv("HOME");
	chdir(args[1]);
}

void	ft_pwd(char **args)
{
	char *buffer;
	size_t	size;
	
	if (args[1] != NULL)
		printf("pwd : too many arguments");
	size = 1024;
	buffer = (char *)malloc(sizeof(char) * size);
	if (buffer == NULL)
		return ; //use EXIT_ERROR
	getcwd(buffer, size);
	printf("%s\n", buffer);
	free (buffer);
}
