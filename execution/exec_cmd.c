/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:31:53 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/06 18:32:27 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/execution.h"

void	execute_single_cmd(t_cmd *cmd, t_exec *exec)
{
	cmd->pid = fork();
	if (cmd->pid < 0)
		error_exit("Fork failed");
	if (cmd->pid == 0)
	{
		if (cmd->in_fd != STDIN_FILENO)
		{
			if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
				error_exit("Dup2 failed (stdin)");
		}
		if (cmd->out_fd != STDOUT_FILENO)
		{
			if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
				error_exit("Dup2 failed (stdout)");
		}
		cmd->path = find_command(cmd->argv[0], exec->envp);
		if (!cmd->path)
			error_exit("Command not found");
		execve(cmd->path, cmd->argv, exec->envp);
		error_exit("Execve failed");
	}
	waitpid(cmd->pid, NULL, 0);
}
