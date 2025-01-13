/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:44:34 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/13 11:34:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **args, t_env *env)
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
	{
		flush_errors("pwd", -1);
		pwd = ft_strdup(sh_getenv(env, "PWD"));
	}
	printf("%s\n", pwd);
	return (free(pwd), 0);
}
