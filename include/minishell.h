/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:17:09 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/02 11:14:13 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termcap.h>

# include "../libft/libft.h"
# include "../libft/ft_printf/ft_printf.h"
# include "../libft/getnextline/get_next_line.h"

# define TRUE 1
# define FALSE 0
# define READ_END 0
# define WRITE_END 1
# define INPUT 0
# define OUTPUT 1
# define T_INPUT 2
# define T_OUTPUT 3

extern char	**environ;//variable global para la funcion de ft_env

/*Enum to match command names input with a number*/
typedef enum cmd_enum
{
	cd,
	pwd,
	unset,
	exprt,
	env,
	echo,
	END
}	t_cmd_enum;
//http://stackoverflow.com/questions/16844728/converting-from-string-to-enum-in-c

typedef int	(*t_func_arr)(char **args);


/*flag bit position: bitvalue = 0 | bitvalue = 1
0: p_line
1: redirs
2: args
3: 
4: 
5: 
6:
7:
*/

typedef struct s_pipe
{
	char	*p_line;
	pid_t	pid;
	char	**redirs;
	char	**rd;
	int		r_fd[4];
	char	**args;
	int		fd[2];
	char	mem_flag;
	struct	s_pipe	*head;
	struct	s_pipe	*prev;
	struct	s_pipe	*next;
}	t_pipe;

typedef struct s_shell
{
	char			*s_line;
	int				token; // 0 = && | 1 = ||
	int				depth;
	int				bracket[2];
	int				p_count;
	struct s_pipe	*pipes;
	struct s_shell	*head;
	struct s_shell	*next;
}	t_shell;


typedef struct	s_env
{
	char 	*value;
	struct s_env	*next;
}	t_env;

typedef struct	s_mini
{
	t_env	*env;
}	t_mini;

static int	g_status;
/*minishell base prompt functions*/
char	**get_cmd_args(char *cmd_in);
int		exec_cmd(char *cmd, char **args, int pcount, char *envp[]);
char	*get_input();
int		start_shell(char *envp[]);
void	exit_minishell(t_shell *sh, int status);
int	main_cmd_return(t_shell *sh, int wstatus);

/* built-ins*/
int 	ft_unset(char **args); 
int		ft_pwd(char **args);
int 	ft_export(char **args);
int		ft_env(char **args);
int		ft_echo(char **args);
int 	ft_cd(char **args); 

/*diverse utils functions*/
void	free_s(void *ptr);
void	free_d(void **ptr);
void	free_pipe(t_pipe *p);
void	free_sh(t_shell *sh);
char	*sh_strtrim(char *str, char *set, char offset);
char	*get_full_path(char *arg0, char *envp[]);
void	set_errno(int err);
void	set_flag(t_pipe *p, int n);
void	unset_flag(t_pipe *p, int n);
/*list utils */
t_shell	*sh_lstnew(char *line);
t_shell	*sh_lstlast(t_shell *sh);
t_shell	*sh_lstadd_back(t_shell **sh, char *line);
t_pipe	*p_lstnew(char **line);
t_pipe	*p_lstlast(t_pipe *pipe);
t_pipe	*p_lstadd_back(t_pipe **pipe, char **line);


/*str utils*/
int		sh_strpos(const char *big, const char *little);
char	*sh_strcut(char *str, int start, int end);
char	*sh_strcut2(char **str, int start, int end);
char	*sh_strstrip(char **str, int start, int end);
char	*sh_strnstr(const char *big, const char *little, size_t len);
char	*sh_strtrim(char *str, char *set, char offset);
void	sh_strtrim2(char **str, char *set, char offset);
void	sh_trim_list_strings(char **str, char *set);
void	clean_spaces(char ***str, char *set); 
int		sh_jump_to(char *str, char c);
int		sh_skip(char *str, char c);

/*signal handlers*/
void 	init_signal(int pid);

/*subshell*/
int		subshell(t_shell *sh, char *envp[]);

#endif
