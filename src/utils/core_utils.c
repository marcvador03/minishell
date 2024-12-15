/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:32:42 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/15 16:56:31 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_minishell.h"

static char	**search_path(char **env)
{
	int		i;
	char	**res;

	i = 0;
	res = NULL;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0 && ft_strlen(env[i]) > 5)
			res = ft_split(env[i] + 5, ':');
		i++;
	}
	return (res);
}

char	*get_full_path(char *arg0, char **env)
{
	char	*cmd_in;
	char	*cmd_out;
	char	**paths;
	int		i;

	if (access(arg0, X_OK) == 0)
		return (arg0);
	paths = search_path(env);
	if (paths == NULL)
		return (set_gstatus(2), NULL);
	cmd_in = ft_strjoin("/", arg0);
	if (cmd_in == NULL)
		return (free_d(paths), set_gstatus(202), NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_out = ft_strjoin(paths[i], cmd_in);
		if (cmd_out == NULL)
			return (free_d(paths), free_s(cmd_in), set_gstatus(202), NULL);
		if (access(cmd_out, X_OK) == 0)
			return (free_d(paths), free_s(cmd_in), cmd_out);
		i++;
		free_s(cmd_out);
	}
	return (free_d(paths), free_s(cmd_in), set_gstatus(127), NULL);
}

void	set_gstatus(int err_code)
{
	g_status = err_code;
}

static void	custom_errors1(char *cmd, int errnum)
{
	g_status = errnum;
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (errnum == 2)
		ft_putendl_fd(E_002, STDERR_FILENO);
	if (errnum == 7)
		ft_putendl_fd(E_007, STDERR_FILENO);
	if (errnum == 8)
		ft_putendl_fd(E_008, STDERR_FILENO);
	if (errnum == 9)
		ft_putendl_fd(E_009, STDERR_FILENO);
	if (errnum == 10)
		ft_putendl_fd(E_010, STDERR_FILENO);
	if (errnum == 11)
		ft_putendl_fd(E_011, STDERR_FILENO);
	if (errnum == 126)
		ft_putendl_fd(E_126, STDERR_FILENO);
	if (errnum == 127)
		ft_putendl_fd(E_127, STDERR_FILENO);
}

static void	custom_errors2(char *cmd, int errnum)
{
	g_status = errnum;
	ft_putstr_fd(cmd, STDERR_FILENO);
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
		custom_errors1(cmd, err_sig);
	else if (err_sig > 200 && err_sig < 256)
		custom_errors2("minishell", err_sig);
	return ;
}

int	max(int n1, int n2)
{
	if (n1 > n2)
		return (n1);
	else
		return (n2);
}
