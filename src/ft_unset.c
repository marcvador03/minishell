/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:29:53 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/18 17:30:08 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_unset(int argc __attribute((unused)), char **argv)
{
	int	status;
	int	i;
	int	j;

	status = 0;
	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j] && env_is_var_char(argv[i][j]))
			j++;
		if (argv[i][j] != '\0' || argv[i][0] == '\0')
		{
			print_error(SHELL_NAME, "unset", argv[i], "not a valid identifier");
			status = EXIT_FAILURE;
		}
		else
			env_unset_var(argv[i]);
		i++;
	}
	return (status);
}
