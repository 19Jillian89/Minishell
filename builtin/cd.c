/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 18:40:04 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 19:12:34 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"
#include "execution.h"
#include <errno.h>
#include <unistd.h>
#include <limits.h>

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

static int	cd_chdir(char *path)
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

static char	*dup_cwd(void)
{
	char	buf[PATH_MAX];

	if (!getcwd(buf, sizeof(buf)))
		return (NULL);
	return (ft_strdup(buf));
}

static void	set_pwd_vars(t_envc *envc, char *oldpwd, char *newpwd)
{
	char	*tmp;

	if (!envc || !newpwd)
		return ;
	if (oldpwd)
	{
		tmp = ft_strjoin("OLDPWD=", oldpwd);
		if (tmp)
			envc->env = set_var(envc->env, tmp);
		free(tmp);
	}
	tmp = ft_strjoin("PWD=", newpwd);
	if (tmp)
		envc->env = set_var(envc->env, tmp);
	free(tmp);
}

int	cd_builtin(char **ac, t_envc *envc)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;
	int		status;

	if (!envc)
		return (1);
	oldpwd = dup_cwd();
	path = create_cdpath(ac, envc->env);
	if (!path)
		return (free(oldpwd), 1);
	status = cd_chdir(path);
	if (status == 0)
	{
		newpwd = dup_cwd();
		set_pwd_vars(envc, oldpwd, newpwd);
		free(newpwd);
	}
	free(oldpwd);
	envc->exit_code = status;
	return (status);
}
