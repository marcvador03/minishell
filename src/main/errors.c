/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:32:42 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/14 10:00:06 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_minishell.h"

void	set_gstatus(int err_code)
{
	g_status = err_code;
}

static void	custom_errors1(char *cmd, int errnum)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (errnum == 7)
		ft_putendl_fd(E_007, STDERR_FILENO);
	if (errnum == 8)
		ft_putendl_fd(E_008, STDERR_FILENO);
	if (errnum == 9)
		ft_putendl_fd(E_009, STDERR_FILENO);
	if (errnum == 11)
		ft_putendl_fd(E_011, STDERR_FILENO);
	if (errnum == 12)
		ft_putendl_fd(E_012, STDERR_FILENO);
	g_status = 1;
}

static void	custom_errors2(char *cmd, int errnum)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
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
	if (errnum == 208)
		ft_putendl_fd(E_208, STDERR_FILENO);
	if (errnum == 209)
		ft_putendl_fd(E_209, STDERR_FILENO);
	g_status = 2;
}

static void	custom_errors3(char *cmd, int errnum)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (errnum == 125)
		ft_putendl_fd(E_125, STDERR_FILENO);
	if (errnum == 126)
		ft_putendl_fd(E_126, STDERR_FILENO);
	if (errnum == 127)
		ft_putendl_fd(E_127, STDERR_FILENO);
	if (errnum == 125)
		g_status = 127;
	g_status = errnum;
}

void	flush_errors(char *cmd, int err_sig)
{
	if (err_sig == -1)
	{
		g_status = 1;
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(cmd);
	}
	else if (err_sig > 2 && err_sig < 126)
		custom_errors1(cmd, err_sig);
	else if (err_sig > 200 && err_sig < 256)
		custom_errors2(cmd, err_sig);
	else if (err_sig >= 125 && err_sig < 128)
		custom_errors3(cmd, err_sig);
	else if (err_sig == 1)
		g_status = 1;
	return ;
}
