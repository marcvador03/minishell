/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:32:42 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/14 18:30:12 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_minishell.h"

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

void	set_gstatus(int err_code)
{
	g_status = err_code;
}

static void	custom_errors(char *cmd, int errnum)
{
	g_status = errnum;
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (errnum == 7)
		ft_putendl_fd(E_007, STDERR_FILENO);
	if (errnum == 8)
		ft_putendl_fd(E_008, STDERR_FILENO);
	if (errnum == 9)
		ft_putendl_fd(E_009, STDERR_FILENO);
	if (errnum == 201)
		ft_putendl_fd(E_201, STDERR_FILENO);
	if (errnum == 202)
		ft_putendl_fd(E_202, STDERR_FILENO);
	if (errnum == 203)
		ft_putendl_fd(E_203, STDERR_FILENO);
	if (errnum == 204)
		ft_putendl_fd(E_204, STDERR_FILENO);
	if (errnum == 205)
		ft_putendl_fd(E_205, STDERR_FILENO);
	if (errnum == 206)
		ft_putendl_fd(E_206, STDERR_FILENO);
	if (errnum == 207)
		ft_putendl_fd(E_207, STDERR_FILENO);
}

void	flush_errors(char *cmd, int err_sig)
{
	if (err_sig == -1)
	{
		g_status = errno;
		ft_putstr_fd("minishell: ", 2);
		perror(cmd);
	}
	else if (err_sig > 1 && err_sig < 128)
		custom_errors(cmd, err_sig);
	else if (err_sig > 200 && err_sig < 256)
		custom_errors("minishell", err_sig);
	return ;
}
