/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:32:47 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/27 00:33:09 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <fcntl.h>
#include <unistd.h>

static int  open_infile(const char *path)
{
    int fd;

    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("minishell: open infile");
        return (-1);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("minishell: dup2 infile");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

static int  open_outfile_trunc(const char *path)
{
    int fd;

    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("minishell: open outfile");
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 outfile");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

static int  open_outfile_append(const char *path)
{
    int fd;

    fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("minishell: open outfile (append)");
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 outfile (append)");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

/*
** Per R_HEREDOC per ora supponiamo che parser ti passi un "target"
** che è un file temporaneo già riempito. Allora lo tratti come un R_IN.
** (Quando farete davvero l'heredoc, basterà adattare questa parte.)
*/
int apply_redirections(t_redir *redirs, t_envc *envc)
{
    while (redirs)
    {
        if (redirs->type == R_IN || redirs->type == R_HEREDOC)
        {
            if (open_infile(redirs->target) < 0)
            {
                envc->exit_code = 1;
                return (-1);
            }
        }
        else if (redirs->type == R_OUT)
        {
            if (open_outfile_trunc(redirs->target) < 0)
            {
                envc->exit_code = 1;
                return (-1);
            }
        }
        else if (redirs->type == R_APP)
        {
            if (open_outfile_append(redirs->target) < 0)
            {
                envc->exit_code = 1;
                return (-1);
            }
        }
        redirs = redirs->next;
    }
    return (0);
}

