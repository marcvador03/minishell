/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:17:09 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/23 15:59:32 by mfleury          ###   ########.fr       */
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
# include <utmp.h>

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

typedef long long		t_ll;
typedef struct s_shell	t_shell;
typedef struct s_env	t_env;
typedef int				(*t_func_arr)(char **args, t_env *env);
extern int				g_status;

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

typedef struct s_parsing
{
	int					i;
	int					j;
	int					k;
	int					prev_pos;
	int					prev_pos2;
	int					beg_sep;
	int					len;
	int					flag_jump;
	int					flag_sep;
	char				**parse;
}	t_parse;

typedef struct s_env
{
	char				*varname;
	char				*value;
	struct s_env		*head;
	struct s_env		*next;
}	t_env;

typedef struct s_prompt
{
	char				*user;
	char				*hostname;
	char				*ex_cur_path;
	char				*cur_path;
	char				*home;
	char				*status;
	char				*prompt;
}	t_prompt;

typedef struct s_redirs
{
	char				**redirs;
	char				**rd;
	int					fd[4];
	int					hd_flag;
	t_shell				*sh;
}	t_redirs;

typedef struct s_pipe
{
	char				*p_line;
	char				**args;
	int					fd[2];
	pid_t				pid;
	int					p_status;
	int					empty_arg;
	int					exit;
	t_shell				*sh;
	t_redirs			*r;
	struct s_pipe		*head;
	struct s_pipe		*prev;
	struct s_pipe		*next;
}	t_pipe;

typedef struct s_termcaps
{
	struct termios		old_term;
	struct termios		new_term;
}	t_terms;

struct s_shell
{
	char				*s_line;
	int					tk;
	int					depth;
	int					bracket[2];
	int					p_count;
	int					exit;
	int					l_status;
	t_redirs			*r;
	t_pipe				*pipes;
	t_terms				*tcap;
	t_env				*env;
	struct s_shell		*head;
	struct s_shell		*next;
	int					level;
	struct s_shell		*up;
	struct s_shell		*down;
};

/* built-ins functions*/
int		ft_unset(char **args, t_env *env);
int		ft_pwd(char **args, t_env *env);
int		ft_export(char **args, t_env *env);
int		ft_env(char **args, t_env *env);
int		ft_echo(char **args, t_env *env);
int		ft_cd(char **args, t_env *env);
int		ft_exit(t_pipe *p, char **args, t_env *env);

/* core utils functions */
void	set_status(int err_code, int *err);
int		max(int n1, int n2);
t_ll	ll_atoi(const char *nptr);
char	*ll_itoa(t_ll n);
int		one_of_char(char c, char *letters);
int		all_of_char(char c, char *letters);
int		none_of_char(char c, char *letters);
void	init_parse(t_parse *q);

/* list utils functions */
t_shell	*sh_lstnew(t_terms *tcap, t_env *env, int *l_status);
t_shell	*sh_lstadd_back(t_shell *sh);
t_shell	*sh_lstadd_down(t_shell *sh);
//t_shell	*sh_lstnew(char *line, t_env *env, int *pos, int *l_status);
//t_shell	*sh_lstadd_back(t_shell **sh, char *line, int *pos, int *l_status);

/* str utils functions */
int		sh_jump_to(char *str, char c);
int		sh_skip(char *str, char c);
char	*sh_trim_spaces(char *str);
int		sh_strpos(const char *big, const char *little);
int		sh_check_empty(char *str);

/* free utils functions */
void	free_s(void *ptr);
void	free_d(char **ptr);
void	free_pipe(t_pipe *p);
void	free_sh(t_shell *sh);
void	free_env(t_env *env);

/* main functions */
void	exit_minishell(t_pipe *p, t_env *env);
void	exit_minishell_error(t_shell *sh, int status, t_env *env);
int		main_cmd_return(t_pipe *p, int wstatus, pid_t pid);
void	init_signal(int pid, int hd);
int		flush_errors(char *cmd, int err_sig, char *tk);
int		check_forbidden_c(char *line);

int	get_fds_redir(t_redirs *r, int *err);
int	close_redir_fd_pipe(t_pipe *p);
int	close_redir_fd_sh(t_shell *sh);

/* environment functions */
int		env_size(t_env *lst);
char	*sh_getenv(t_env *env, char *str);
t_env	*fill_env(char *envp[]);
t_env	*sh_updateenv(t_env *env, char *var_name, char *new_value);
t_env	*sh_addenv(t_env *env, char *var_name, char *value);
t_env	*sh_delenv(t_env *env, char *var_name);
char	*expand_env(t_env *env, char *line, int x, int l_status);
char	**get_env_array(t_env *env, int *err);
t_env	*fill_default_session(t_env *env);
char	*get_full_path(char *arg0, t_env *env, int *err);

/*term caps*/
void	set_term_settings(t_terms *tcap, t_env *env);
void	unset_term_settings(t_terms *tcap, t_env *env);
void	init_termcaps(t_terms *tcap, t_env *env);

#endif
