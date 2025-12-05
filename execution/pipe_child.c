/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:55:21 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/27 00:55:27 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void exec_with_path(t_exec_cmd *cmd, t_envc *envc)
{
    char *path = find_command(cmd->argv[0], envc->env);

    if (!path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->argv[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    execve(path, cmd->argv, envc->env);
    perror("minishell: execve");
    exit(126);
}

static void prepare_child_fds(t_pipeline *p, int i, int prev_read, int pipefd[2])
{
    if (handle_input_fd(prev_read) < 0)
        exit(1);
    if (handle_output_fd(i == (int)p->count - 1, pipefd) < 0)
        exit(1);
}

void exec_pipeline_child(t_pipeline *p, t_envc *envc,
                         int i, int prev_read, int pipefd[2])
{
    setup_signals_child();
    prepare_child_fds(p, i, prev_read, pipefd);
    if (apply_redirections(p->cmds[i]->redirs, envc) != 0)
        exit(envc->exit_code);
    if (!p->cmds[i]->argv || !p->cmds[i]->argv[0])
        exit(0);
    exec_with_path(p->cmds[i], envc);
}

