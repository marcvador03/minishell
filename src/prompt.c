/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:26:04 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/02 00:23:27 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*create_prompt(void)
{
	char	*user;
	char	*res;

	user = getenv("USER");
	if (user == NULL)
		return (set_errno(ENODATA), NULL);
	res = ft_strjoin(user, "$ ");
	if (res == NULL)
		return (set_errno(ENOMEM), NULL);
	return (res);
}

char	**identify_pipes(char *s_line, t_pipe **p)
{
	char	**pipes;
	int		i;

	pipes = ft_split(s_line, '|');
	if (pipes == NULL)
		return (set_errno(ENOMEM), NULL);
	(*p)->count = 0;
	i = 0;
	while (pipes[i++] != NULL)
		(*p)->count++;
	return (set_flag(*p, 0), pipes);
}

int	count_p(char *line, int cnt)
{
	char	*str;

	if (ft_strchr(line, '(') == 0 && ft_strchr(line, ')') == 0)
		return (free_s(line), 1);
	else if (ft_strchr(line, '(') > ft_strrchr(line, ')'))
		return (free_s(line), 0);
	while (*line != '\0')
	{
		if (*line == '(')
		{
			str = ft_substr(line + 1, 0, ft_strrchr(line, ')') - line);
			if (count_p(str, cnt) == 0)
				return (free_s(line), 0);
			cnt += count_p(str, cnt);
			break;
		}
		line++;
	}
	return (free_s(line), cnt);
}

int	sh_strpos(const char *big, const char *little)
{
	size_t	i;
	int		j;
	char 	*str;

	str = (char *)big;
	if (ft_strlen(little) == 0)
		return (ft_strlen(big));
	i = 0;
	while (*str != '\0')
	{
		if (*little == *str)
		{
			j = 0;
			while (little[j] == str[j])
				if (little[++j] == '\0')
					return (i);
		}
		str++;
		i++;
	}
	return (ft_strlen(big));
}

char	*sh_strcut(char *str, int start, int end)
{
	char	*res;
	int		i;
	int		j;
	int		len;

	if (str == NULL || end <= start)
		return (NULL);
	len = ft_strlen(str);
	if (start > len)
		return (NULL);
	if (end - start > len)
		end = start + len;
	res = (char *)ft_calloc(sizeof(char), end - start + 1);
	if (res == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (i < end && str[i] != '\0')
		res[j++] = str[i++];
	res[j] = '\0';
	return (res);
}

int	get_next_token(t_shell *sh, char *line)
{
	int		len;
	char 	*tk;
	char	*t_line;

	t_line = line + 2;
	len = ft_strlen(t_line);
	if (sh_strpos(t_line, "&&") == len && sh_strpos(t_line, "||") == len)
		tk = NULL;
	else if (sh_strpos(t_line, "&&") < sh_strpos(t_line, "||"))
		tk = "&&";
	else if (sh_strpos(t_line, "||") < sh_strpos(t_line, "&&"))
		tk = "||";
	sh->s_line = sh_strcut(line, 2, sh_strpos(t_line, tk) + 2);
	if (sh->s_line == NULL)
		return (set_errno(ENOMEM), ENOMEM);
	if (ft_strncmp(line, "||", 2) == 0) 
		sh->token = 1;
	else if (ft_strncmp(line, "&&", 2) == 0) 
		sh->token = 0;
	sh->pipes->in_pipes = identify_pipes(sh->s_line, &sh->pipes);
	return (0);	
}

char	*get_input()
{
	char	*line;
	char	*line2;
	char	*prompt;

	prompt = create_prompt();
	if (prompt == NULL)
		return (set_errno(ENOMEM), NULL);
	line = readline(prompt);
	if (line == NULL)
		return (free_s(prompt), set_errno(ENOMEM), NULL);
	else if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), get_input());
	add_history(line);
	line2 = ft_strjoin("&&", line);
	return (free_s(prompt), free_s(line), line2);
}

int	count_tokens(char *line)
{
	int		n;
	int		i;
	int		len;
	char	*t_line;

	if (line == NULL)
		return (0);
	n = 0;
	t_line = line + 2;
	len = ft_strlen(t_line);
	if (sh_strpos(t_line, "&&") == len && sh_strpos(t_line, "||") == len)
		return (1);
	i = 0;
	while (i < len)
	{
		if (sh_strpos(t_line + i, "&&") < sh_strpos(t_line + i, "||"))
			i += sh_strpos(t_line + i, "&&") + 2;
		else	
			i += sh_strpos(t_line + i, "||") + 2;
		n++;
	}
	return (n);
}

t_shell	*fill_sh(t_shell *sh, char *line, int n)
{
	int		i;
	t_shell	*tmp;
	
	i = 0;
	while (i++ < n && *line != '\0')
	{
		if (sh == NULL)
			tmp = sh_lstnew(line);
		else
			tmp = sh_lstadd_back(&sh, line);
		if (tmp == NULL)
			return (NULL);
		line = line + ft_strlen(tmp->s_line) + 2;
		free_s((void *)tmp->s_line);
	//	tmp->s_line = ft_strtrim(tmp->s_line, " ");
		sh = tmp->head;
	}
	return (tmp->head);
}

void	main_cmd_return(t_shell *sh)
{
	if (errno > 0 && errno < 255)
		exit_minishell(sh, EXIT_FAILURE);
	else if (errno == 255 && sh->pipes->count == 1)
		exit_minishell(sh, EXIT_SUCCESS);
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
		perror("minishell: ");	
	n = count_tokens(line);
	sh = fill_sh(sh, line, n);
	if (sh == NULL)
		return (set_errno(ENOMEM), free_s((void *)line), ENOMEM);
	free_s((void *)line);
	errno = 0;
	head = sh->head;
	while (sh != NULL)
	{
		if (sh->token == 0 || (sh->token == 1 && errno != 0))
		{
			errno = subshell(sh->pipes, envp);
			main_cmd_return(head);
		}
		sh = sh->next;
	}
//	main_cmd_return(sh);
	return (free_sh(head), 0);
}
