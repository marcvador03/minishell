/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 09:59:33 by mfleury           #+#    #+#             */
/*   Updated: 2024/09/16 11:12:22 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
# if BUFFER_SIZE > 10000
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif
# define OPEN_MAX 1024
# define FALSE 0
# define TRUE 1
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "../libft.h"

typedef struct s_gnl_list
{
	char				*content;
	int					eol;
	int					fd;
	struct s_gnl_list	*next;
	struct s_gnl_list	*bof;
}	t_gnl_list;
char		*get_next_line(int fd);
t_gnl_list	*gnl_lst_move(t_gnl_list *lst, int n, int fd);
t_gnl_list	*gnl_free(t_gnl_list *lst);
t_gnl_list	*gnl_new(char *content, t_gnl_list *head, int fd);
char		*gnl_substr(char const *s, unsigned int start, size_t len);
#endif
