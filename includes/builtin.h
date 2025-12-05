/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 01:52:01 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:45:33 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../libft/libft.h"
# include "minishell.h"
# include "execution.h"
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>

/* Forward declaration */
typedef struct s_exec_cmd t_exec_cmd;

/* BUILTIN CHECK  */
int     is_builtin(char *cmd);
int     execute_builtin(t_exec_cmd *cmd, t_envc *envc);

/* BUILTINS */
int     cd_builtin(char **ac, t_envc *envc);
int     pwd_builtin(void);
int     echo_builtin(char **argv);
int     env_builtin(t_exec_cmd *cmd, t_envc *envc);
int     exit_builtin(char **args, t_envc *envc);
int     unset_builtin(t_exec_cmd *cmd, t_envc *envc);
int     export_builtin(t_exec_cmd *cmd, t_envc *envc);
char    **set_var(char **env, const char *entry);
void    export_sorted(char **env);

/* UTILITY */
char    *get_env_value(char **env, char *key);
int     ft_strcmp(const char *s1, const char *s2);

#endif
