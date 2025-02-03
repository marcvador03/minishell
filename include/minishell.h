/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmorello <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:17:09 by pmorello          #+#    #+#             */
/*   Updated: 2025/02/03 21:12:53 by mfleury          ###   ########.fr       */
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
	int					status;
	char				**parse;
	char				*t_line;
	char				tk;
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
	int					p_count;
	int					exit;
	int					l_status;
	t_redirs			*r;
	t_pipe				*pipes;
	t_terms				*tcap;
	t_env				*env;
	struct s_shell		*head;
	struct s_shell		*next;
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
int		max(int n1, int n2);
char	*ll_itoa(t_ll n);
t_ll	ll_atoi(const char *nptr);
void	init_parse(t_parse *q);
int		ft_isalnum_plus(int c);
int		ft_isalnum_plus_q(int c);
void	set(int err_code, int *err);
void	swap(char **str1, char **str2);

/* str utils functions */
int		sh_jump_to(char *str, char c);
int		sh_skip(char *str, char c);
char	*sh_trim_spaces(char *str);
int		sh_strpos(const char *big, const char *little);
int		sh_check_empty(char *str);
int		oneofchar(char c, char *letters);
int		noneofchar(char c, char *letters);

/* free utils functions */
void	free_s(void *ptr);
void	free_d(char **ptr);
void	free_pipe(t_pipe *p);
void	free_sh(t_shell *sh);

/* main functions */
int		flush_errors(char *cmd, int err_sig, char tk);
int		main_cmd_return(t_pipe *p, int wstatus, pid_t pid);
void	exit_minishell_error(t_shell *sh, int errnum, t_env *env);
void	exit_minishell(t_pipe *p, t_env *env);
void	init_signal(int pid, int hd);

/* environment functions */
char	**get_env_array(t_env *env, int *err);
char	*sh_getenv(t_env *env, char *str, int flag);
t_env	*sh_addenv(t_env *env, char *var_name, char *value);
t_env	*sh_updateenv(t_env *env, char *var_name, char *new_value);
t_env	*sh_delenv(t_env *env, char *var_name);
int		env_size(t_env *lst);
char	*expand_variable(t_shell *sh, char *line, int *i);
t_env	*fill_env(char *envp[]);

/* path functions */
int		is_absolute_path(char *cmd, int *err);
int		check_file_directory(char *t_cmd, int *err);
char	*get_full_path(char *arg0, t_env *env, int *err);

/* parsing functions 
int		create_separation(char *line, t_parse *q);
int		create_rd_separation(char *line, t_parse *q);
int		separate_quotes(char *line, t_parse *q);
int		separate_dollar(char *line, t_parse *q);*/

/* file descriptor functions */
int		open_redir_fd(t_redirs *r, int *err, char *cmd);
int		close_redir_fd_sh(t_shell *sh);
int		close_redir_fd_pipe(t_pipe *p);
int		close_redir_fd_single(t_redirs *r, int *err, char *cmd);
int		close_pipes(t_pipe *p);
int		get_fds_redir(t_redirs *r, int *err);
int		get_rd_flag(char *rd);

/*term caps*/
void	set_term_settings(t_terms *tcap, t_env *env);
void	unset_term_settings(t_terms *tcap, t_env *env);
void	init_termcaps(t_terms *tcap, t_env *env);

#endif
