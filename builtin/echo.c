/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 02:32:28 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/25 02:44:47 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <unistd.h>

static int	check_n_option(char *arg)
{
	if (!arg || arg[0] != '-')
		return (0);
	arg++;
	if (!*arg)
		return (0);
	while (*arg)
	{
		if (*arg != 'n')
			return (0);
		arg++;
	}
	return (1);
}

int	echo_builtin(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;

	while (argv[i] && check_n_option(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
