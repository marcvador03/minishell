/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:26:04 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/06 15:30:27 by mfleury          ###   ########.fr       */
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
		return (ft_strlen(big) + 1);
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
	return (ft_strlen(big) + 1);
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

void	count_brackets(t_shell *sh, char *line)
{
	char	*t_line;
	
	t_line = line;
	while (*ft_strchr(t_line, '(') != '\0' && *t_line != '\0')
	{
		sh->bracket[0]++;
		t_line = ft_strchr(t_line, '(') + 1;
	}
	sh->bracket[1] = sh->bracket[0];
	t_line = line;
	while (*ft_strchr(t_line, ')') != '\0' && *t_line != '\0')
	{
		sh->bracket[1]--;
		t_line = ft_strchr(t_line, ')') + 1;
	}

}

char	*get_tk(char *line)
{
	int		len;
	
	len = ft_strlen(line);
	if (sh_strpos(line, "&&") == len && sh_strpos(line, "||") == len)
		return (NULL);
	else if (sh_strpos(line, "&&") < sh_strpos(line, "||"))
		return ("&&");
	else if (sh_strpos(line, "||") < sh_strpos(line, "&&"))
		return ("||");
	return (NULL);
}

int	get_next_token(t_shell *sh, char *line)
{
	char 	*tk;
	char	*t_line;
	
	t_line = ft_strtrim(ft_strtrim(line," ") + 2, " ");
	tk = get_tk(t_line);
	sh->s_line = sh_strcut(line, 2, sh_strpos(t_line, tk) + 2);
	sh->s_line = ft_strtrim(sh->s_line, " ");
	count_brackets(sh, sh->s_line);
	sh->s_line = ft_strtrim(sh->s_line, "(");
	sh->s_line = ft_strtrim(sh->s_line, ")");
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
		i += sh_strpos(t_line + i, get_tk(t_line + i)) + 2;
		n++;
	}
	return (n);
}

t_shell	*fill_sh(t_shell *sh, char *line, int n)
{
	int		i;
	t_shell	*tmp;
	int		x[2];
	char	*tk;
	
	i = 0;
	ft_memset(x, 0, 2 * sizeof(int));
	while (i++ < n && *line != '\0')
	{
		line = ft_strtrim(line, " ");
		if (sh == NULL)
			tmp = sh_lstnew(line);
		else
			tmp = sh_lstadd_back(&sh, line);
		if (tmp == NULL)
			return (NULL);
		tmp->s_line = ft_strtrim(tmp->s_line, " ");
		tmp->bracket[0] += x[0];
		tmp->bracket[1] += x[1];
		tk = get_tk(line + 2);
		line = ft_strnstr(line + 2, tk, ft_strlen(line));
		x[0] = tmp->bracket[0];
		x[1] = tmp->bracket[1];
	//	free_s((void *)tmp->s_line);
		sh = tmp->head;
		
	}
	return (tmp->head);
}

int	main_cmd_return(t_shell *sh, int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		if (sh->pipes->mem_flag & (1 << 7))
			exit_minishell(sh, EXIT_SUCCESS);
		if (WEXITSTATUS(wstatus) > 0 && WEXITSTATUS(wstatus) < 255)
			perror("minishell: ");
	}
	return (0);
}

void	get_bracket(char *line)
{
	char	*t_line;

	t_line = line;
	
	t_line = ft_strrchr(line, '(');
	t_line = ft_substr(t_line, 1, sh_strpos(t_line, ")"));
	if (t_line == NULL)
		return ;
}

int	execute_tokens(t_shell *sh, t_shell *head, int level, char *envp[])
{
	pid_t	pid;
	int		wstatus;
	int		errnum;

	errnum = 0;
	while (sh != NULL)
	{
		pid = 0;
		if (sh->bracket[0] > level)
		{
			pid = fork();
			if (pid == -1)
				perror("minishell: ");
			if (pid == 0)
				exit(execute_tokens(sh, head, ++level, envp));
			waitpid(pid, &wstatus, 0);
			return(main_cmd_return(sh, wstatus));
		}
		else if (sh->bracket[0] == level)
		{
			if (sh->token == 0 || (sh->token == 1 && errnum != 0))
			{
				errnum = subshell(sh, sh->pipes, envp);
				if (errnum == -1)
					perror("minishell: ");
			}
		}
		else if (sh->bracket[1] < level)
			exit(0);
		sh = sh->next;
	}
	return(0);
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
