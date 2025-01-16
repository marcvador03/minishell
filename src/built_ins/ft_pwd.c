/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:44:34 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/16 10:26:53 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_option(char *str, char *cmd);

int	ft_pwd(char **args, t_env *env)
{
	char	*pwd;
	int		err;

	err = 0;
	if (env == NULL)
		return (207);
	err = check_option(args[1], "pwd");
	if (err == 2)
		return (err);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		flush_errors("pwd", -1);
		pwd = ft_strdup(sh_getenv(env, "PWD"));
	}
	printf("%s\n", pwd);
	return (free(pwd), err);
}
