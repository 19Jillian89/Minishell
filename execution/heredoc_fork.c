/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:16:54 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:11:37 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

static int	handle_child_status(int status, char *filename, t_envc *envc)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		envc->exit_code = 130;
		unlink(filename);
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		envc->exit_code = 1;
		unlink(filename);
		return (-1);
	}
	return (0);
}

static int	fork_heredoc(char *del, char *filename, t_envc *envc, int exp)
{
	pid_t	pid;
	int	status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork heredoc");
		return (-1);
	}
	if (pid == 0)
	{
		setup_signals_child();
		if (heredoc_child_process(del, filename, envc, exp) != 0)
			exit(1);
		exit(0);
	}
	waitpid(pid, &status, 0);
	return (handle_child_status(status, filename, envc));
}

static void	cleanup_heredoc(char *delim, char *filename)
{
	free(delim);
	free(filename);
}

int	create_heredoc_for_redir(t_redir *redir, t_envc *envc)
{
	char	*filename;
	char	*delim;
	int		expand;

	expand = !heredoc_quote(redir->target);
	delim = remove_quotes(redir->target);
	if (!delim)
		return (-1);
	filename = tmp_heredoc(envc);
	if (!filename)
	{
		free(delim);
		return (-1);
	}
	if (fork_heredoc(delim, filename, envc, expand) == -1)
	{
		cleanup_heredoc(delim, filename);
		return (-1);
	}
	free(delim);
	free(redir->target);
	redir->target = filename;
	redir->type = R_IN;
	return (0);
}
