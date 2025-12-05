/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 18:15:35 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/25 19:02:10 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	valid_identifier(const char *key)
{
	int	i;

	if (!key || !key[0])
		return (0);
	if (!(ft_isalpha((unsigned char)key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum((unsigned char)key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	var_remove(char **env, const char *name)
{
	int	i;
	int	j;
	size_t		len;

	if (!env || !name)
		return ;
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0
			&& (env[i][len] == '=' || env[i][len] == '\0'))
		{
			free(env[i]);
			j = i;
			while (env[j + 1])
			{
				env[j] = env[j + 1];
				j++;
			}
			env[j] = NULL;
			continue ;
		}
		i++;
	}
}

int	unset_builtin(t_exec_cmd *cmd, t_envc *envc)
{
	int	i;
	int	status;

	if (!cmd || !cmd->argv || !envc || !envc->env)
		return (0);
	i = 1;
	status = 0;
	while (cmd->argv[i])
	{
		if (!valid_identifier(cmd->argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			var_remove(envc->env, cmd->argv[i]);
		i++;	
	}
	envc->exit_code = status;
	return (status);
}
