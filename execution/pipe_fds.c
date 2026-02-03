/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:54:53 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 18:34:53 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void	prepare_child_fds(t_pipeline *p, int i, t_pipe_status *status)
{
	if (handle_input_fd(status->prev_read) < 0)
		exit(1);
	if (handle_output_fd(i == (int)p->count - 1, status->pipefd) < 0)
		exit(1);
}

int	handle_input_fd(int prev_read)
{
	if (prev_read == -1)
		return (0);
	if (dup2(prev_read, STDIN_FILENO) == -1)
		return (perror("minishell: dup2 prev_read"), -1);
	close(prev_read);
	return (0);
}

int	handle_output_fd(int is_last, int pipefd[2])
{
	if (is_last)
		return (0);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		return (perror("minishell: dup2 pipefd[1]"), -1);
	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}

void	close_parent_fds(int prev_read, int pipefd[2], int is_last)
{
	if (prev_read != -1)
		close(prev_read);
	if (!is_last)
		close(pipefd[1]);
}
