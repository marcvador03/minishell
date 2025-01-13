/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_minishell.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:17:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/13 11:43:36 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MINISHELL_H
# define ERROR_MINISHELL_H

# define E_007 ": too many arguments"
# define E_008 ": $HOME is not set"
# define E_009 ": $OLDPWD is not set"
# define E_010 ": export variable name is not valid"
# define E_011 ": unset variable name is not valid"
# define E_012 ": numeric argument expected"
# define E_125 ": no such file or directory"
# define E_126 ": Is a directory"
# define E_127 ": command not found"

# define E_200 ": unrecoverable error due terminal setup"
# define E_201 ": missing quotes \" or \'"
# define E_202 ": error encountered during memory allocation"
# define E_203 ": error near redirection token \'<\' or \'>\'"
# define E_204 ": error near token \'&&\' or \'||\'"
# define E_205 ": error near pipe token \'|\'"
# define E_206 ": error near parenthesis \'(\' or \')\'"
# define E_207 ": error while fetching env value"
# define E_208 ": target directory does not exist"
# define E_209 ": too many arguments"

#endif
