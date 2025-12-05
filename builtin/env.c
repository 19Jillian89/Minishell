/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 02:52:57 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/25 03:03:39 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <unistd.h>

int	env_builtin(t_exec_cmd *cmd, t_envc *envc)
{
	int	i;

	if (cmd->argv[1])
	{
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return (1);
	}
	if (!envc || !envc->env)
		return (0);
	i = 0;
	while (envc->env[i])
	{
		if (ft_strchr(envc->env[i], '='))
		{
			write(1, envc->env[i], ft_strlen(envc->env[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

