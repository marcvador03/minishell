/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:44:34 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/17 20:00:22 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **args, char ***env)
{
	char	*pwd;

	if (env == NULL)
		return (207);
	if (args[1] != NULL)
	{
		if (args[1][0] == '-')
			return (7);
	}
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		pwd = ft_strdup(sh_getenv(*env, "PWD"));
	printf("%s\n", pwd);
	return (free(pwd), 0);
}
