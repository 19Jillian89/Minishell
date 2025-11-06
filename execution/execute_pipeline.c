/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:45:37 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/06 19:03:48 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ----------------------------------------------------------- */
/*      Esegue il codice del figlio con le giuste redirezioni  */
/* ----------------------------------------------------------- */
void	child_process(t_cmd *cmd, t_exec *exec, int first, int last)
{
	if (!first)
	{
		if (dup2(exec->pipefd[0], STDIN_FILENO) == -1)
			error_exit("Dup2 failed (stdin)");
	}
	if (!last)
	{
		if (dup2(exec->pipefd[1], STDOUT_FILENO) == -1)
			error_exit("Dup2 failed (stdout)");
	}
	close(exec->pipefd[0]);
	close(exec->pipefd[1]);
	cmd->path = find_command(cmd->argv[0], exec->envp);
	if (!cmd->path)
		error_exit("Command not found");
	execve(cmd->path, cmd->argv, exec->envp);
	error_exit("Execve failed");
}

/* ----------------------------------------------------------- */
/*          Esegue una pipeline di N comandi                   */
/* ----------------------------------------------------------- */
void	execute_pipeline(t_cmd **cmds, int cmd_count, t_exec *exec)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		if (i < cmd_count - 1 && pipe(exec->pipefd) == -1)
			error_exit("Pipe failed");
		cmds[i]->pid = fork();
		if (cmds[i]->pid < 0)
			error_exit("Fork failed");
		if (cmds[i]->pid == 0)
			child_process(cmds[i], exec, i == 0, i == cmd_count - 1);
		if (i < cmd_count - 1)
			close(exec->pipefd[1]);
		if (i > 0)
			close(exec->pipefd[0]);
		i++;
	}
	while (wait(NULL) > 0)
		;
}
