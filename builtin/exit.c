/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 17:15:15 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/25 17:55:18 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <unistd.h>

static int	numeric_value(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (!ft_isdigit((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

static int	exit_args(char **args, t_envc *envc)
{
	long	code;

	if (!numeric_value(args[1]))
	{
		write(2, "exit\n", 5);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric arg required\n", 2);
		envc->exit_code = 2;
		envc->should_exit = 1;
		return (envc->exit_code);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		envc->exit_code = 1;
		envc->should_exit = 0;
		return (1);
	}
	code = ft_atoi(args[1]);
	write(2, "exit\n", 5);
	envc->exit_code = (unsigned char)code;
	envc->should_exit = 1;
	return (envc->exit_code);
}

int	exit_builtin(char **args, t_envc *envc)
{
	if (!args || !args[0])
		return (0);
	if (!args[1])
	{
		write(2, "exit\n", 5);
		envc->should_exit = 1;
		return (envc->exit_code);
	}
	return (exit_args(args, envc));
}
