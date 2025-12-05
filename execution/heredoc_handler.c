/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:31:24 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/04 16:31:27 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"

static int	process_cmd_redirs(t_exec_cmd *cmd, t_envc *envc)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == R_HEREDOC)
		{
			if (create_heredoc_for_redir(redir, envc) != 0)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

int	handle_heredocs(t_pipeline *p, t_envc *envc)
{
	int		i;

	if (!p)
		return (0);
	i = 0;
	while (i < (int)p->count)
	{
		if (process_cmd_redirs(p->cmds[i], envc) == -1)
			return (-1);
		i++;
	}
	return (0);
}
