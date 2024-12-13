/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:34:22 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/13 16:27:51 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_shell *sh, char **args)
{
	int	i;

	i = 0;
	if (args[1] != NULL && args[2] != NULL)
		return (flush_errors("exit", 7));
	else if (args[1] != NULL)
	{
		while (args[1][i] != 0)
		{
			if (ft_isdigit(args[1][i]) == 0)
			{
				if (args[1][i] != ' ' && args[1][i] != '-')
					return (flush_errors("exit", 7));
			}
			i++;
		}
	}
	g_status = 0;
	if (args[1] != NULL)
		g_status = ft_atoi(args[1]);
	exit_minishell(sh);
	return ;
}

