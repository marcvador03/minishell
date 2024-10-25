/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:32:42 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/25 14:11:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**search_path(char *envp[])
{
	int		i;
	char	**res;

	i = 0;
	res = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0 && ft_strlen(envp[i]) > 5)
			res = ft_split(envp[i] + 5, ':');
		i++;
	}
	return (res);
}

char	*get_full_path(char *arg0, char *envp[])
{
	char	*cmd_in;
	char	*cmd_out;
	char	**paths;
	int		i;

	if (access(arg0, F_OK) == 0)
		return (arg0);
	paths = search_path(envp);
	if (paths == NULL)
		return (NULL);
	cmd_in = ft_strjoin("/", arg0);
	if (cmd_in == NULL)
		return (free_d((void **)paths), NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_out = ft_strjoin(paths[i], cmd_in);
		if (cmd_out == NULL)
			return (free_d((void **)paths), free_s(cmd_in), NULL);
		if (access(cmd_out, F_OK) == 0)
			return (free_d((void **)paths), free_s(cmd_in), cmd_out);
		i++;
		free_s(cmd_out);
	}
	return (free_d((void **)paths), free_s(cmd_in), NULL);
}

void	set_errno(int err)
{
	errno = err;
}

void	set_flag(t_shell *sh, int n)
{
	sh->flag = sh->flag | (1 << n);
}

void	unset_flag(t_shell *sh, int n)
{
	sh->flag = sh->flag & ~ (1 << n);
}
