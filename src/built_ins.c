/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:36:03 by pmorello          #+#    #+#             */
/*   Updated: 2024/10/14 13:14:30 by mfleury          ###   ########.fr       */
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
