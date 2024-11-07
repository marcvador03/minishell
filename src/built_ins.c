/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:36:03 by pmorello          #+#    #+#             */
/*   Updated: 2024/11/07 12:17:00 by pmorello         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_cd(char **args)
{
	char	*path;

	if (args[1] != NULL && args[2] != NULL)
		return(E2BIG);
	if (args[1] == NULL)
		path = getenv("HOME");
	else
		path = args[1];
	chdir(path);
	return (0);
}

void	ft_echo(int argc, char **argv)
{
	size_t	len;
	int	i;
	int new_line;

	new_line = 1;
	i = 1;
	len = ft_strlen(argv[i]);
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "-n", len) == 0)
			new_line = 0;
		else
		{
			printf("%s", argv[i]);
			if (i < argc - 1)
				printf (" ");
		}
		i++;
	}
	if (new_line)
		printf("\n");
}

int	ft_pwd(char **args)
{
	char *buffer;
	size_t	size;
	
	if (args[1] != NULL)
		return (E2BIG);
//		printf("pwd : too many arguments");
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
