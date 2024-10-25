/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:17:09 by pmorello          #+#    #+#             */
/*   Updated: 2024/10/25 14:16:17 by mfleury          ###   ########.fr       */
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
typedef enum cmd_enum
{
	cd,
	pwd,
	END
}	t_cmd_enum;
//http://stackoverflow.com/questions/16844728/converting-from-string-to-enum-in-c
extern char	**environ;//variable global para la funcion de ft_env

typedef int	(*t_func_arr)(char **args);
typedef struct s_shell
{
	pid_t	*pid;
	char	**in_pipes;
	char	***args;
	int		**fd;
	int		count;
	char	flag;
}	t_shell;

/*minishell base prompt functions*/
char	*create_prompt(void);
char	**get_cmd_args(char *cmd_in);
int		exec_cmd(char **args, char *envp[]);
char	**get_input(t_shell *sh);

/* minishell built-ins functions*/
int		ft_cd(char **args);
int		ft_pwd(char **args);
void	ft_env(int argc, char **argv);
void	ft_echo(int argc, char **argv);

/*diverse utils functions*/
void	free_s(void *ptr);
void	free_d(void **ptr);
void	free_sh(t_shell *sh);
char	*get_full_path(char *arg0, char *envp[]);
void	set_errno(int err);
void	set_flag(t_shell *sh, int n);
void	unset_flag(t_shell *sh, int n);

/*subshell*/
int		subshell(t_shell *sh, char *envp[]);
#endif
