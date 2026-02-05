/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:30:33 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 16:22:54 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

/*
update_exit_status_from_wait
-----------------------------
Interprets the exit status returned by waitpid() and updates envc->exit_code.

@status: status returned by waitpid().
@envc: environment and exit-code structure.
- If the child exited normally, sets exit_code to its exit value.
- If terminated by a signal, sets exit_code according to shell conventions:
     SIGINT  → 130
     SIGQUIT → 131 + prints "Quit (core dumped)"
     Other   → 128 + signal number
*/
static void	update_exit_status_from_wait(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->envc.exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			shell->envc.exit_code = 130;
			ft_putstr_fd("\n", 1);
		}
		else if (WTERMSIG(status) == SIGQUIT)
		{
			shell->envc.exit_code = 131;
		}
		else
			shell->envc.exit_code = 128 + WTERMSIG(status);
	}
}

/*
execute_single_cmd
-------------------
Executes a command that is NOT part of a pipeline (a standalone command).

@cmd: structure containing command arguments and possible redirections.
@envc: environment and exit-code structure.

Forks a child process to execute the command, applies correct signal
behavior, waits for termination, and updates the exit code accordingly.
Handles fork() and waitpid() errors gracefully.
*/
void	execute_single_cmd(t_exec_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	setup_signals_parent_exec();
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		shell->envc.exit_code = 1;
		setup_signals_interactive();
		return ;
	}
	if (pid == 0)
		exec_external_child(cmd, shell);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
		shell->envc.exit_code = 1;
	}
	else
		update_exit_status_from_wait(status, shell);
	setup_signals_interactive();
}
