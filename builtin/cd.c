/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 18:40:04 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/25 02:24:43 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"
#include "execution.h"
#include <errno.h>

static char	*create_cdpath(char **ac, char **env)
{
	char	*path;
	
	if (ac[1] && ac[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (NULL);
	}
	if (!ac[1])
	{
		path = get_env_value(env, "HOME");
		if (!path)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (path);
	}
	return (ac[1]);
}

static int cd_chdir(char *path)
{
    if (chdir(path) == -1)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(path, 2);
	ft_putstr_fd(":", 2);
        ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
        return (1);
    }
    return (0);
}

int	cd_builtin(char **ac, t_envc *envc)
{
	char	*path;
	int	status;

	if (!envc)
		return (1);
	path = create_cdpath(ac, envc->env);
	if (!path)
		return (1);
	status = cd_chdir(path);
	envc->exit_code = status;
	return (status);
}
