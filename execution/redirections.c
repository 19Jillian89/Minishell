/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:32:47 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 18:55:05 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <fcntl.h>
#include <unistd.h>

static void	ms_perror_path(const char *path)
{
	write(2, "minishell: ", 11);
	write(2, path, ft_strlen(path));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}

static int	open_infile(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("minishell: open infile");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ms_perror_path(path);
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	open_outfile_trunc(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell: open outfile");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ms_perror_path(path);
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	open_outfile_append(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ms_perror_path(path);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ms_perror_path(path);
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_redir *redirs, t_shell *shell)
{
	int	ret;

	while (redirs)
	{
		ret = 0;
		if (redirs->type == R_IN || redirs->type == R_HEREDOC)
			ret = open_infile(redirs->target);
		else if (redirs->type == R_OUT)
			ret = open_outfile_trunc(redirs->target);
		else if (redirs->type == R_APP)
			ret = open_outfile_append(redirs->target);
		if (ret < 0)
		{
			shell->envc.exit_code = 1;
			return (-1);
		}
		redirs = redirs->next;
	}
	return (0);
}
/*
Per R_HEREDOC per ora supponiamo che parser ti passi un "target"
che è un file temporaneo già riempito. Allora lo tratti come un R_IN.
(Quando farete davvero l'heredoc, basterà adattare questa parte.)
*/
/*int	apply_redirections(t_redir *redirs, t_shell *shell)
{
	while (redirs)
	{
		if (redirs->type == R_IN || redirs->type == R_HEREDOC)
		{
			if (open_infile(redirs->target) < 0)
			{
				shell->envc.exit_code = 1;
				return (-1);
			}
		}
		else if (redirs->type == R_OUT)
		{
			if (open_outfile_trunc(redirs->target) < 0)
			{
				shell->envc.exit_code = 1;
				return (-1);
			}
		}
		else if (redirs->type == R_APP)
		{
			if (open_outfile_append(redirs->target) < 0)
			{
				shell->envc.exit_code = 1;
				return (-1);
			}
		}
		redirs = redirs->next;
	}
	return (0);
}*/
