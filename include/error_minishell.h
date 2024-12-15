/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_minishell.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:17:52 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/15 11:31:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MINISHELL_H
# define ERROR_MINISHELL_H

# define E_007 ": too many arguments"
# define E_008 ": $HOME is not set"
# define E_009 ": $OLDPWD is not set"
# define E_010 ": export variable name is not valid"

# define E_200 ": error encountered during terminal setup"
# define E_201 ": missing quotes \" or \'"
# define E_202 ": error encountered during memory allocation"
# define E_203 ": error near redirection token \'<\' or \'>\'"
# define E_204 ": error near token \'&&\' or \'||\'"
# define E_205 ": error near pipe token \'|\'"
# define E_206 ": error near parenthesis \'(\' or \')\'"
# define E_207 ": error while fecthing env value"

#endif
