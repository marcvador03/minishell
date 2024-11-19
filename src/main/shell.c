/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/19 14:39:49 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_tokens(char *line);
int		execute_tokens(t_shell *sh, t_shell *head, int level, char *envp[]);
char	*get_tk(char *line);

t_shell	*fill_sh(t_shell *sh, char *line, int n)
{
	int		i;
	t_shell	*tmp;
	int		x[2];
	char	*tk;
	char	*t_line;

	i = 0;
	ft_memset(x, 0, 2 * sizeof(int));
	while (i++ < n && *line != '\0')
	{
		t_line = ft_strtrim(line, " ");
		if (sh == NULL)
			tmp = sh_lstnew(t_line);
		else
			tmp = sh_lstadd_back(&sh, t_line);
		if (tmp == NULL)
			return (NULL);
		tmp->s_line = sh_strtrim(&tmp->s_line, " ", 0);
		tmp->bracket[0] += x[0];
		tmp->bracket[1] += x[1];
		tk = get_tk(line + 2);
		line = ft_strnstr(line + 2, tk, ft_strlen(line));
		x[0] = tmp->bracket[0];
		x[1] = tmp->bracket[1];
		sh = tmp->head;
	}
	return (tmp->head);
}

int	start_shell(char *envp[])
{
	char	*line;
	t_shell	*sh;
	t_shell	*head;
	int		n;

	n = 0;
	sh = NULL;
	line = get_input();
	if (line == NULL)
		return (1);
	n = count_tokens(line);
	sh = fill_sh(sh, line, n);
	if (sh == NULL)
		return (free_s((void *)line), 1);
	free_s((void *)line);
	head = sh->head;
	if (execute_tokens(sh, head, 0, envp) != 0)
		return (free_sh(head), 1);
	return (free_sh(head), 0);
}
