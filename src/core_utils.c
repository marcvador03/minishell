/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:32:42 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/24 00:47:47 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_s(void *ptr)
{

	free(ptr);
	ptr = NULL;
}

void	free_d(void **ptr)
{
	int	i;

	i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		ptr[i++] = NULL;
	}
	free(ptr);
	ptr = NULL;
}

void	free_sh(t_shell *sh)
{
	int	i;
	
	if (sh->free_flag & (1 << 4))
	{
		free(sh->pid);
		sh->pid = NULL;
	}
	if (sh->free_flag & (1 << 1))
		free_d((void **)sh->in_pipes);
	if (sh->free_flag & (1 << 3))
		free_d((void **)sh->fd);
	if (sh->free_flag & (1 << 2))
	{
		i = 0;
		while (sh->args[i] != NULL)
			free_d((void **)sh->args[i++]);
		free(sh->args);
		sh->args = NULL;
	}
	sh->free_flag = 0;
}

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
			return (free_d((void **)paths), free(cmd_in), NULL);
		if (access(cmd_out, F_OK) == 0)
			return (free_d((void **)paths), free(cmd_in), cmd_out);
		i++;
		free(cmd_out);
	}
	return (free_d((void **)paths), free_s(cmd_in), NULL);
}

void	set_errno(int err)
{
	errno = err;
}

void	set_flag(t_shell *sh, int n)
{
	sh->free_flag = sh->free_flag | (1 << n); 	
}

void	unset_flag(t_shell *sh, int n)
{
	sh->free_flag = sh->free_flag & ~ (1 << n); 	
}