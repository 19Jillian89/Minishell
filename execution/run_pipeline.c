/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipeline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 23:14:34 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/04 13:52:32 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "builtin.h"

static void	save_stdio_fds(int *in_backup, int *out_backup)
{
	*in_backup = dup(STDIN_FILENO);
	*out_backup = dup(STDOUT_FILENO);
}

static void	restore_stdio_fds(int in_backup, int out_backup)
{
	if (in_backup != -1)
	{
		dup2(in_backup, STDIN_FILENO);
		close(in_backup);
	}
	if (out_backup != -1)
	{
		dup2(out_backup, STDOUT_FILENO);
		close(out_backup);
	}
}

/* Esegue un builtin nel processo padre, gestendo anche le redirections.*/
static int	exec_builtin_in_parent(t_exec_cmd *cmd, t_envc *envc)
{
	int	in_backup;
	int	out_backup;
	int	status;

	save_stdio_fds(&in_backup, &out_backup);
	if (in_backup == -1 || out_backup == -1)
	{
		perror("minishell: dup");
		envc->exit_code = 1;
		return (1);
	}
	if (apply_redirections(cmd->redirs, envc) != 0)
	{
		restore_stdio_fds(in_backup, out_backup);
		return (envc->exit_code);
	}
	status = execute_builtin(cmd, envc);
	envc->exit_code = status;
	restore_stdio_fds(in_backup, out_backup);
	return (status);
}

void	run_pipeline(t_pipeline *p, t_envc *envc)
{
	t_exec_cmd	*cmd;

	if (!p || p->count == 0)
		return ;
	if (handle_heredocs(p, envc) != 0)
		return ;
	if (p->count == 1)
	{
		cmd = p->cmds[0];
		if (!cmd || !cmd->argv || !cmd->argv[0])
			return ;
		if (is_builtin(cmd->argv[0]))
			exec_builtin_in_parent(cmd, envc);
		else
			execute_single_cmd(cmd, envc);
	}
	else
		exec_pipeline(p, envc);
}

/*void	run_pipeline(t_pipeline *p, t_envc *envc)
{
	t_exec_cmd	*cmd;

	if (!p || p->count == 0)
		return ;
	if (p->count == 1)
	{
		cmd = p->cmds[0];
		if (!cmd || !cmd->argv || !cmd->argv[0])
			return ;
		if (is_builtin(cmd->argv[0]))
			exec_builtin_in_parent(cmd, envc);
		else
		{
			setup_signals_parent_exec();
			execute_single_cmd(cmd, envc);
		}
		else
		{
			setup_signals_parent_exec();
			exec_pipeline(p, envc);
		}
	}
}*/
