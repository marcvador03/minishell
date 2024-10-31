/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:17:09 by pmorello          #+#    #+#             */
/*   Updated: 2024/10/30 19:09:27 by mfleury          ###   ########.fr       */
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

# define TRUE 1
# define FALSE 0

extern char	**environ;//variable global para la funcion de ft_env

/*Enum to match command names input with a number*/
typedef enum cmd_enum
{
	cd,
	pwd,
	END
}	t_cmd_enum;
//http://stackoverflow.com/questions/16844728/converting-from-string-to-enum-in-c

typedef int	(*t_func_arr)(char **args);
/*flag bit position: bitvalue = 0 | bitvalue = 1
0: "&&" | "||" 
1: no exit | exit minishell
2: pipes mem freed | pipes allocated on heap
3: args mem freed | args allocated on heap
4: fd mem freed | fd allocated on heap
5: pid mem freed | pid allocated on heap
6:
7:
*/

typedef struct s_pipe
{
	pid_t	*pid;
	char	**in_pipes;
	char	***args;
	int		**fd;
	int		count;
	char	mem_flag;
}	t_pipe;

typedef struct s_shell
{
	char			*s_line;
	int				token; // 0 = && | 1 = ||
	int				depth;
	struct s_pipe	*pipes;
	struct s_shell	*head;
	struct s_shell	*next;
}	t_shell;

/*minishell base prompt functions*/
char	*create_prompt(void);
char	**get_cmd_args(char *cmd_in);
int		exec_cmd(char **args, char *envp[]);
char	*get_input();
void	start_shell(char *envp[]);

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
/*list utils */
t_shell	*sh_lstnew(char *line);
t_shell	*sh_lstlast(t_shell *sh);
void	sh_lstadd_back(t_shell **sh, char *line);

/*subshell*/
int		subshell(t_shell *sh, char *envp[]);
#endif
