/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/12 18:44:53 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tk(char *line);
int		count_tokens(char *line);
int		check_open_quotes(char *str);
int		execute_tokens(t_shell *sh, t_shell *head, int level, char *envp[]);
int		check_input(char *line);
int		init_data_brackets(t_shell *tmp, int *a, int *b);


static t_shell	*fill_sh(t_shell *sh, char *line, int n, t_terms *tcap)
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
		if (init_data_brackets(tmp, &x[0], &x[1]) == -1)
			return (set_gstatus(202), NULL);
		tk = get_tk(line + 2);
		line = sh_strnstr(line + 2, tk, ft_strlen(line));
		sh = tmp->head;
		sh->tcap = tcap;
	}
	return (tmp->head);
}

static char	*create_prompt(void)
{
	char	*user;
	char	*tmp;
	char	*res;
	char	*status;

	user = getenv("USER");
	if (user == NULL)
		return (NULL);
	res = ft_strjoin(user, "$(");
	status = ft_itoa(g_status);
	if (res == NULL || status == NULL)
		return (NULL);
	tmp = ft_strjoin(res, status);
	if (tmp == NULL)
		return (free_s(res), free_s(status), NULL);
	free_s(res);
	res = ft_strjoin(tmp, ")> ");
	if (res == NULL)
		return (free_s(tmp), free_s(status), NULL);
	return (free_s(tmp), free_s(status), res);
}

static char	*get_input(void)
{
	char	*line;
	char	*line2;
	char	*prompt;

	prompt = create_prompt();
	if (prompt == NULL)
		return (NULL);
	line = readline(prompt);
	if (line == NULL)
		return (free_s(prompt), NULL);
	else if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), set_gstatus(0), get_input());
	else if (check_input(line) != 0)
		return (free_s(prompt), free_s(line), set_gstatus(0), get_input());
	add_history(line);
	line2 = ft_strjoin("&&", line);
	return (free_s(prompt), free_s(line), line2);
}

int	start_shell(char *envp[], t_terms *tcap)
{
	char	*line;
	t_shell	*sh;
	t_shell	*head;
	int		n;

	n = 0;
	sh = NULL;
	init_signal(1, 0);
	line = get_input();
	if (line == NULL)
		exit_minishell(sh, 0);
	if (check_open_quotes(line) == -1)
		return (free_s((void *)line), set_gstatus(201), -1);
	n = count_tokens(line);
	sh = fill_sh(sh, line, n, tcap);
	if (sh == NULL)
		return (free_s((void *)line), -1);
	free_s((void *)line);
	head = sh->head;
	g_status = 0;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	if (execute_tokens(sh, head, 0, envp) != 0)
		return (free_sh(head), 1);
	return (free_sh(head), 0);
}
