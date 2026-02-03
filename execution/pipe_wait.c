/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:55:44 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 18:41:54 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	update_exit_status(int status, t_envc *envc)
{
	if (WIFEXITED(status))
		envc->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			envc->exit_code = 130;
			ft_putstr_fd("\n", 1);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			envc->exit_code = 131;
			ft_putstr_fd("Quit (core dumped)\n", 2);
		}
		else
			envc->exit_code = 128 + WTERMSIG(status);
	}
}

void	wait_pipeline(pid_t last_pid, t_envc *envc)
{
	pid_t	pid;
	int		status;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
			update_exit_status(status, envc);
		pid = wait(&status);
	}
}
