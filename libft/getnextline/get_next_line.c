/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 09:56:13 by mfleury           #+#    #+#             */
/*   Updated: 2024/09/12 00:15:47 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*gnl_out_line(t_gnl_list **lst, int fd)
{
	t_gnl_list	*tmp;
	char		*res[2];	

	res[0] = NULL;
	tmp = *lst;
	*lst = (*lst)->bof;
	while ((*lst != NULL))
	{
		if ((*lst)->fd == fd)
		{
			res[1] = res[0];
			res[0] = ft_strjoin(res[0], (*lst)->content);
			free(res[1]);
			if (*lst == tmp)
			{
				*lst = gnl_free(*lst);
				return (res[0]);
			}
			*lst = gnl_free(*lst);
		}
		else
			*lst = (*lst)->next;
	}
	return (res[0]);
}

static t_gnl_list	*gnl_fill(char *tmp, t_gnl_list *lst, int fd, ssize_t buf)
{
	char	*ptr;
	int		s;
	int		l;

	tmp[buf] = '\0';
	ptr = tmp;
	while (*ptr != '\0')
	{
		s = (int)(ptr - tmp);
		if (*ft_strchr(ptr, '\n') != '\0')
			ptr = (ft_strchr(tmp + (ptr - tmp), '\n') + 1);
		else
			ptr = (ft_strchr(tmp + (ptr - tmp), '\0'));
		l = (int)(ptr - tmp) - s - 1;
		if (lst == NULL)
			lst = gnl_new(gnl_substr(tmp, s, l), NULL, fd);
		else
		{
			lst = gnl_lst_move(lst, -1, -1);
			lst->next = gnl_new(gnl_substr(tmp, s, l), lst->bof, fd);
		}
		if (lst == NULL || (lst != lst->bof && lst->next == NULL))
			lst = gnl_free(lst);
	}
	return (lst);
}

char	*get_next_line(int fd)
{
	ssize_t				buffer;
	static t_gnl_list	*lst;
	char				*str;

	str = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (str == NULL || fd < 0 || fd > OPEN_MAX || BUFFER_SIZE <= 0)
		return (free(str), NULL);
	buffer = read(fd, str, BUFFER_SIZE);
	if ((buffer == 0 && lst == NULL) || buffer < 0)
		return (free(str), NULL);
	lst = gnl_lst_move(lst, -1, fd);
	if (buffer > 0)
		lst = gnl_fill(str, lst, fd, buffer);
	if (buffer == 0 && lst->fd == fd)
		lst->eol = TRUE;
	lst = lst->bof;
	while (lst->eol != TRUE || (lst->eol == TRUE && lst->fd != fd))
	{
		if (lst->next == NULL && buffer > 0)
			return (free(str), get_next_line(fd));
		else if (lst->next == NULL && buffer == 0)
			return (free(str), NULL);
		lst = lst->next;
	}
	return (free(str), gnl_out_line(&lst, fd));
}
