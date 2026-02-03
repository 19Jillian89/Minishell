/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:55:21 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 18:32:55 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "builtin.h"
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static void	ms_err_cmd(const char *cmd, const char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd((char *)msg, 2);
	ft_putstr_fd("\n", 2);
}

static int	exec_error_code(const char *path, int err)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (126);
	if (err == EACCES || err == EISDIR)
		return (126);
	if (err == ENOENT)
		return (127);
	return (126);
}

static void	execve_fail_exit(char *path)
{
	int	err;
	int	code;

	err = errno;
	code = exec_error_code(path, err);
	ms_err_cmd(path, strerror(err));
	free(path);
	exit(code);
}

static void	exec_with_path(t_exec_cmd *cmd, t_envc *envc)
{
	char		*path;
	struct stat	st;

	path = find_command(cmd->argv[0], envc->env);
	if (!path)
	{
		ms_err_cmd(cmd->argv[0], "command not found");
		exit(127);
	}
	if (ft_strchr(path, '/'))
	{
		if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			ms_err_cmd(path, "Is a directory");
			free(path);
			exit(126);
		}
	}
	execve(path, cmd->argv, envc->env);
	execve_fail_exit(path);
}

void	exec_pipeline_child(t_pipeline *p, t_shell *shell,
			int i, t_pipe_status *status)
{
	setup_signals_child();
	prepare_child_fds(p, i, status);
	if (apply_redirections(p->cmds[i]->redirs, shell) != 0)
		exit(shell->envc.exit_code);
	if (!p->cmds[i]->argv || !p->cmds[i]->argv[0])
		exit(0);
	if (is_builtin(p->cmds[i]->argv[0]))
		exit(execute_builtin(p->cmds[i], shell));
	exec_with_path(p->cmds[i], &shell->envc);
}
