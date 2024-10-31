/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:26:04 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/30 19:29:54 by mfleury          ###   ########.fr       */
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

char	**identify_pipes(t_shell *sh)
{
	char	**pipes;
	int		i;

	pipes = ft_split(sh->s_line, '|');
	if (pipes == NULL)
		return (set_errno(ENOMEM), NULL);
	sh->count = 0;
	i = 0;
	while (pipes[i++] != NULL)
		sh->count++;
	return (pipes);
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
			{
				j++;
				if (little[j] == '\0')
					return (i);
			}
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
	while (i < end || str[i] != '\0')
		res[j++] = str[i++];
	res[j] = '\0';
	return (res);
}

int	get_next_token(t_shell *sh, char *line)
{
	int		len;
	char 	*tk;

	len = ft_strlen(line);
	if (sh_strpos(line, "&&") == len && sh_strpos(line, "||") == len)
		tk = NULL;
	else if (sh_strpos(line, "&&") < sh_strpos(line, "||"))
		tk = "&&";
	else if (sh_strpos(line, "||") < sh_strpos(line, "&&"))
	{	
		sh->token = 1;
		tk = "||";
	}
	sh->s_line = sh_strcut(line, 0, sh_strpos(line, tk));
	sh->pipes->in_pipes = identify_pipes(sh);//
	return (0);	
}

char	*get_input(t_shell *sh)
{
	char	*line;
	char	*prompt;

	prompt = create_prompt();
	if (prompt == NULL)
		return (set_errno(ENOMEM), NULL);
	line = readline(prompt);
	if (line == NULL)
		return (free_s(prompt), set_errno(ENOMEM), NULL);
	else if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), get_input(sh));
	add_history(line);
	return (free_s(prompt), set_flag(sh, 2), line);
}

int	count_tokens(char *line)
{
	int	n;
	int	i;
	int	len;

	if (line == NULL)
		return (0);
	n = 1;
	len = ft_strlen(line);
	if (sh_strpos(line, "&&") == len && sh_strpos(line, "||") == len)
		return (n);
	i = 0;
	while (i <= len)
	{
		if (sh_strpos(line + i, "&&") < sh_strpos(line + i, "||"))
			i += sh_strpos(line + i, "&&");
		else	
			i += sh_strpos(line + i, "||");
		n++;
	}
	return (n);
}

void	start_shell(char *envp[])
{
	char	*line;
	t_shell	*sh;
	char	*t_line;
	int		n;
	int		i;

	n = 0;
	i = 0;
	line = get_input(sh);
	if (line == NULL)
		perror("minishell: ");	
	n = count_tokens(line);
	t_line = line;
	while (i++ < n)
	{
		if (sh == NULL)
			sh = sh_lstnew(t_line);
		else
			sh_lstadd_back(&sh, t_line);
		t_line = line +  
	}
	free(line);
}
