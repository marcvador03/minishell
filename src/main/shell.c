/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/14 18:45:02 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tk(char *line);
int		count_tokens(char *line);
int		check_open_quotes(char *str);
int		execute_tokens(t_shell *sh, int i, int level, char **env);
int		check_input(char *line);
int		init_data_brackets(t_shell *tmp, int *a, int *b, char **env);

static t_shell	*fill_sh(t_shell *sh, char *line, t_terms *tcap, char **env)
{
	int		i;
	t_shell	*tmp;
	int		x[2];
	char	*t_line;
	int		n;

	i = 0;
	n = count_tokens(line);
	ft_memset(x, 0, 2 * sizeof(int));
	while (i++ < n && *line != '\0')
	{
		t_line = line + sh_skip(line, ' ');
		if (sh == NULL)
			tmp = sh_lstnew(t_line);
		else
			tmp = sh_lstadd_back(&sh, t_line);
		if (tmp == NULL)
			return (NULL);
		if (init_data_brackets(tmp, &x[0], &x[1], env) == -1)
			return (set_gstatus(202), NULL);
		sh = tmp->head;
		sh->tcap = tcap;
		if (sh->tcap == NULL )
			return (NULL);
	}
	return (tmp->head);
}

static char	*create_prompt(char **env)
{
	char	*user;
	char	*pwd;
	char	*tmp;
	char	*res;
	//char	*status;

	user = sh_getenv(env, "USER");
	if (user == NULL)
		return (set_gstatus(207), NULL);
	res = ft_strjoin(user, ":");
	//status = ft_itoa(g_status);
	pwd = sh_getenv(env, "PWD");
	if (pwd == NULL)
		return (set_gstatus(207), NULL);
	tmp = ft_strjoin(res, pwd);
	if (tmp == NULL)
		return (free_s(res), NULL);
	free_s(res);
	res = ft_strjoin(tmp, "$ ");
	if (res == NULL)
		return (free_s(tmp), NULL);
	return (free_s(tmp), res);
}

static char	*get_input(char **env)
{
	char	*line;
	char	*line2;
	char	*prompt;

	prompt = create_prompt(env);
	if (prompt == NULL)
		return (NULL);
	line = readline(prompt);
	if (line == NULL)
		return (free_s(prompt), NULL);
	else if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), set_gstatus(0), get_input(env));
	else if (check_input(line) != 0)
		return (free_s(prompt), free_s(line), set_gstatus(0), get_input(env));
	add_history(line);
	line2 = ft_strjoin("&&", line);
	return (free_s(prompt), free_s(line), line2);
}

int	start_shell(char **env, t_terms *tcap)
{
	char	*line;
	t_shell	*sh;
	t_shell	*head;

	sh = NULL;
	init_signal(1, 0);
	line = get_input(env);
	if (line == NULL && g_status == 0)
		exit_minishell(sh, env);
	else if (line == NULL && g_status != 0)
		exit_minishell_error(sh, g_status, env);
	if (check_open_quotes(line) == -1)
		return (free_s((void *)line), set_gstatus(201), -1);
	sh = fill_sh(sh, line, tcap, env);
	if (sh == NULL)
		return (free_s((void *)line), -1);
	free_s((void *)line);
	if (sh->bracket[1] != 0)
		return (free_sh(sh->head), set_gstatus(206), -1);
	head = sh->head;
	g_status = 0;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	if (execute_tokens(sh, 0, 0, env) != 0)
		return (free_sh(head), 1);
	return (free_sh(head), 0);
}
