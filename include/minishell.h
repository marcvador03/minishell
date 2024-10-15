/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:17:09 by pmorello          #+#    #+#             */
/*   Updated: 2024/10/15 22:12:20 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../libft/libft.h"
# include "../libft/ft_printf/ft_printf.h"
# include "../libft/getnextline/get_next_line.h"

/*Enum to match command names input with a number*/
typedef enum {cd, pwd, END} cmd_enum;
//http://stackoverflow.com/questions/16844728/converting-from-string-to-enum-in-c

typedef	void (*t_func_arr)(char **args);

/* minishell core utils */
char	*create_prompt();
void	ft_cd(char **args);
void	ft_pwd(char **args);

extern char **environ; //variable global para la funcion de ft_env

#endif
