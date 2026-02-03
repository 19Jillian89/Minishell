/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 16:05:19 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 16:24:10 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

static void	ms_err_cmd(const char *cmd, const char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd((char *)msg, 2);
	ft_putstr_fd("\n", 2);
}

static int	exec_error_code(int err)
{
	if (err == EACCES || err == EISDIR)
		return (126);
	if (err == ENOENT)
		return (127);
	return (126);
}

static void	exec_fail_exit(char *path)
{
	int	err;
	int	code;

	err = errno;
	code = exec_error_code(err);
	ms_err_cmd(path, strerror(err));
	free(path);
	exit(code);
}

void	exec_external_child(t_exec_cmd *cmd, t_shell *shell)
{
	char		*path;
	struct stat	st;

	setup_signals_child();
	if (apply_redirections(cmd->redirs, shell) != 0)
		exit(shell->envc.exit_code);
	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	path = find_command(cmd->argv[0], shell->envc.env);
	if (!path)
	{
		ms_err_cmd(cmd->argv[0], "command not found");
		exit(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ms_err_cmd(path, "Is a directory");
		free(path);
		exit(126);
	}
	execve(path, cmd->argv, shell->envc.env);
	exec_fail_exit(path);
}
