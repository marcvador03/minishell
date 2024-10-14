/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:36:03 by pmorello          #+#    #+#             */
/*   Updated: 2024/10/14 12:11:17 by pmorello         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//void echo -n

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
		printf("pwd : too many arguments\n");
	
}

//void	ft_export

//void	ft_unset

void	ft_env(int argc, char **argv)
{
	//usar la variable global environ
	char **env;
	
	env = environ;
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	if (argc > 1)
		printf("env: '%s' : No such file or directori\n", argv[1]);
}

void	ft_exit(void)
{
	exit(0);
}
