/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:45:37 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 17:05:17 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/*
create_pipe_if_needed
----------------------
Creates a pipe if the current command is not the last one in the pipeline.
@is_last: whether this command is the last in the pipeline.
@pipefd: array where the new pipe file descriptors will be stored.
@prev_read: fd of the previous pipe read end, closed in case of error.
@envc: environment and exit-code structure.
If a new pipe is needed and creation fails, prints an error, closes the
previous read-end if open, sets the exit code to 1 and returns -1.
Return: 0 on success, -1 on failure.
*/
static int	create_pipe_if_needed(int is_last, int pipefd[2],
				int prev_read, t_envc *envc)
{
	if (!is_last)
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell: pipe");
			if (prev_read != -1)
				close(prev_read);
			envc->exit_code = 1;
			return (-1);
		}
	}
	return (0);
}

/*
handle_fork_error
------------------
Handles cleanup when fork() fails during pipeline execution.
@is_last: whether this command is the last in the pipeline.
@pipefd: pipe descriptors created for this command.
@prev_read: previous pipe read end to be closed.
@envc: environment and exit-code structure.
Closes necessary descriptors, sets the exit code to 1 and returns -1.
Return: always -1.
*/
static int	handle_fork_error(int is_last, int pipefd[2],
				int prev_read, t_envc *envc)
{
	perror("minishell: fork");
	if (!is_last)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (prev_read != -1)
		close(prev_read);
	envc->exit_code = 1;
	return (-1);
}

/*
is_last_cmd
------------
Returns 1 if the command at index `i` is the last one in the pipeline.
@p: pipeline structure.
@i: index of the current command.
Return: 1 if last, otherwise 0.
*/
static int	is_last_cmd(t_pipeline *p, int i)
{
	if (i == (int)p->count - 1)
		return (1);
	return (0);
}

/*
run_one_child
--------------
Executes a single command in the pipeline by handling pipes, forking,
file descriptor setup, and tracking the last child's PID.
@p: pipeline structure containing all commands.
@envc: environment and exit-code structure.
@i: index of the current pipeline command.
@prev_read: pointer to the previous pipe's read-end fd.
@last_pid: pointer to where the last child's PID will be stored.
Creates a new pipe if necessary, forks a new child, calls the child
execution function, and updates prev_read and last_pid for the next iteration.
Return: 0 on success, -1 on failure.
*/
static int	run_one_child(t_pipeline *p, t_shell *shell,
		int i, t_pipe_status *status)
{
	int		is_last;
	pid_t	pid;

	is_last = is_last_cmd(p, i);
	status->pipefd[0] = -1;
	status->pipefd[1] = -1;
	if (create_pipe_if_needed(is_last, status->pipefd, status->prev_read,
			&shell->envc) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(is_last, status->pipefd, status->prev_read,
				&shell->envc));
	if (pid == 0)
		exec_pipeline_child(p, shell, i, status);
	close_parent_fds(status->prev_read, status->pipefd, is_last);
	if (is_last)
		status->prev_read = -1;
	else
		status->prev_read = status->pipefd[0];
	status->last_pid = pid;
	return (0);
}

/*
exec_pipeline
--------------
Executes an entire pipeline of commands.
@p: pipeline structure containing all commands.
@envc: environment and exit-code structure.
Initializes parent signal handlers, iterates through all pipeline commands,
launching children connected by pipes. After the loop, closes the last
remaining file descriptor, waits for the last command (to set the exit code),
and restores interactive shell signals.
*/
void	exec_pipeline(t_pipeline *p, t_shell *shell)
{
	int				i;
	t_pipe_status	status;	

	if (!p || p->count == 0)
		return ;
	setup_signals_parent_exec();
	status.prev_read = -1;
	status.last_pid = -1;
	i = 0;
	while (i < (int)p->count)
	{
		if (run_one_child(p, shell, i, &status) == -1)
			break ;
		i++;
	}
	if (status.prev_read != -1)
		close(status.prev_read);
	if (status.last_pid != -1)
		wait_pipeline(status.last_pid, &shell->envc);
	setup_signals_interactive();
}
