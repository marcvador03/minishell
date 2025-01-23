/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_minishell.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:17:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 23:21:20 by mfleury          ###   ########.fr       */
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
# define E_013 ": permission denied"
# define E_125 ": no such file or directory"
# define E_126 ": Is a directory"
# define E_127 ": command not found"

# define E_200 ": unrecoverable error due terminal setup"
# define E_202 ": error encountered during memory allocation"
# define E_208 ": target directory does not exist"
# define E_209 ": too many arguments"
# define E_210 ": syntax error around:'"
# define E_211 ": global syntax error preventing parsing"

#endif
