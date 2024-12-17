/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:17:09 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/17 10:13:52 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termcap.h>
# include <termios.h>

# include "../libft/libft.h"
# include "../libft/getnextline/get_next_line.h"

# define TRUE 1
# define FALSE 0
# define READ_END 0
# define WRITE_END 1
# define INPUT 0
# define OUTPUT 1
# define T_INPUT 2
# define T_OUTPUT 3
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

extern unsigned int	g_status;

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

typedef int			(*t_func_arr)(char **args, char ***env);

typedef struct s_pipe
{
	char			*p_line;
	pid_t			pid;
	char			**redirs;
	char			**rd;
	int				r_fd[4];
	char			**args;
	int				fd[2];
	struct s_pipe	*head;
	struct s_pipe	*prev;
	struct s_pipe	*next;
}	t_pipe;

typedef struct s_termcaps
{
	struct termios	old_term;
	struct termios	new_term;
	char			*keys_on;
	char			*keys_off;
	char			*up_arrow;
	char			*down_arrow;
	char			*backspace;
	char			*del_line;
	char			*set_cursor_begin;
}	t_terms;

typedef struct s_shell
{
	char			*s_line;
	int				token;
	int				depth;
	int				bracket[4];
	int				p_count;
	t_pipe			*pipes;
	t_terms			*tcap;
	struct s_shell	*head;
	struct s_shell	*next;
}	t_shell;

typedef struct s_env
{
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_mini
{
	t_env	*env;
}	t_mini;

/* built-ins functions*/
int		ft_unset(char **args, char ***env);
int		ft_pwd(char **args, char ***env);
int		ft_export(char **args, char ***env);
int		ft_env(char **args, char ***env);
int		ft_echo(char **args, char ***env);
int		ft_cd(char **args, char ***env);
int		ft_unset(char **args, char ***env);
void	ft_exit(t_shell *sh, char **args, char **env);

/* core utils functions */
char	*get_full_path(char *arg0, char **env);
void	set_gstatus(int err_code);
int		max(int n1, int n2);
/* str utils functions */
void	sh_trim_list_strings(char **str);
char	*sh_trim_strings(char *s);
int		sh_strpos(const char *big, const char *little);
int		sh_jump_to(char *str, char c);
int		sh_jump_to2(char **str, char c);
int		sh_skip(char *str, char c);
int		sh_check_empty(char *str);

/* list utils functions */
t_shell	*sh_lstnew(char *line);
t_shell	*sh_lstadd_back(t_shell **sh, char *line);
t_pipe	*p_lstnew(char *line);
t_pipe	*p_lstadd_back(t_pipe **pipe, char *line);

/* free utils functions */
void	free_s(void *ptr);
void	free_d(char **ptr);
void	free_pipe(t_pipe *p);
void	free_sh(t_shell *sh);

/* main functions */
void	exit_minishell(t_shell *sh, char **env);
void	exit_minishell_error(t_shell *sh, int status, char **env);
int		main_cmd_return(t_pipe *p, int wstatus);
void	init_signal(int pid, int hd);
void	flush_errors(char *cmd, int err_sig);

/* environment functions */
char	*sh_getenv(char **env, char *str);
char	**fill_env(char *envp[]);
char	**sh_update_env(char ***env, char *str, char *new_value);
char	**sh_add_env(char ***env, char *str, char *new_value);
char	**sh_del_env(char ***env, char *str);

/*term caps*/
void	set_term_settings(t_terms *tcap, char **env);
void	unset_term_settings(t_terms *tcap, char **env);
void	init_termcaps(t_terms *tcap, char **env);

#endif
