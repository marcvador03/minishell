/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/12 00:30:39 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_minishell.h"

int	start_shell(char *envp[]);
int	g_status = 0;

void	custom_errors(int errnum)
{
	g_status = errnum;
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
}

void	flush_errors(char *cmd, int err_sig)
{
	if (err_sig == -1)
	{
		g_status = errno;
		ft_putstr_fd("minishell: ", 2);
		perror(cmd);
	}
	/*else if (err_sig > 0 && err_sig < 32)
		g_status = errno + 128;*/
	else if (err_sig > 200 && err_sig < 256)
		custom_errors(err_sig);
	return ;
}

int	main_cmd_return(char *cmd, int wstatus)
{
	if (WIFSIGNALED(wstatus) != 0)
		flush_errors(cmd, WTERMSIG(wstatus));
	return (0);
}

void	exit_minishell_error(t_shell *sh, int status)
{
	ft_putstr_fd("minishell exited with error: ", 2);
	ft_putnbr_fd(status, 2);
	ft_putstr_fd("\n", 2);
	free_sh(sh);
	exit(status);
}

void	exit_minishell(t_shell *sh, int status)
{
	printf("minishell exited with success\n");
	free_sh(sh);
	exit(g_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_terms	tcap;	
	
	g_status = 0;
	if (isatty(STDIN_FILENO) == 0)
		exit_minishell_error(NULL, errno);
	init_termcaps(&tcap);
	set_term_settings(&tcap);
	init_signal(1);
	if (argc > 1 || argv == NULL)
		exit_minishell_error(NULL, 1);
	while (1)
		if (start_shell(envp) != 0)
			flush_errors(NULL, g_status);
	return (0);
}
