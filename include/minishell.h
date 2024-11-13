/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:17:09 by pmorello          #+#    #+#             */
/*   Updated: 2024/11/13 13:31:26 by mfleury          ###   ########.fr       */
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
# include <termcap.h>

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
0: pipes mem freed | pipes allocated on heap
1: args mem freed | args allocated on heap
2: fd mem freed | fd allocated on heap
3: pid mem freed | pid allocated on heap
4: redirs mem freed | redirs allocated on heap
5: cmd mem freed | cmd allocated on heap
6:
7:
*/

typedef struct s_pipe
{
	pid_t	*pid;
	char	**in_pipes;
	char	**cmd;
	char	***redirs;
	int		**rd;
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
	int				bracket[2];
	struct s_pipe	*pipes;
	struct s_shell	*head;
	struct s_shell	*next;
}	t_shell;

/*minishell base prompt functions*/
char	**get_cmd_args(char *cmd_in);
int		exec_cmd(char *cmd, char **args, char *envp[]);
char	*get_input();
int		start_shell(char *envp[]);
void	exit_minishell(t_shell *sh, int status);
int	main_cmd_return(t_shell *sh, int wstatus);

/* minishell built-ins functions*/
int		ft_cd(char **args);
int		ft_pwd(char **args);
void	ft_env(int argc, char **argv);
void	ft_echo(int argc, char **argv);

/*diverse utils functions*/
void	free_s(void *ptr);
void	free_d(void **ptr);
void	free_pipe(t_pipe *p);
void	free_sh(t_shell *sh);
char	*sh_strtrim(char **str, char *set, char offset);
char	*get_full_path(char *arg0, char *envp[]);
void	set_errno(int err);
void	set_flag(t_pipe *p, int n);
void	unset_flag(t_pipe *p, int n);
/*list utils */
t_shell	*sh_lstnew(char *line);
t_shell	*sh_lstlast(t_shell *sh);
t_shell	*sh_lstadd_back(t_shell **sh, char *line);

/*str utils*/
int		sh_strpos(const char *big, const char *little);
char	*sh_strcut(char *str, int start, int end);
char	*sh_strcut2(char **str, int start, int end);
char	*sh_strstrip(char **str, int start, int end);
void	clean_spaces(char **s); 

/*subshell*/
int		subshell(t_shell *sh, t_pipe *p, char *envp[]);


#endif
