/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:32:06 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/03 17:32:17 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>

/*Free the argv with str_arr_free(cmd->argv, -1)
Traverse the t_redir list and free the target + node
Free the t_exec_cmd struct itself.
Useful for:
Intermediate errors,
Final free after execution.*/
void	free_exec_cmd(t_exec_cmd *cmd)
{
	t_redir	*r;
	t_redir	*next;

	if (!cmd)
		return ;
	str_arr_free(cmd->argv, -1);
	r = cmd->redirs;
	while (r)
	{
		next = r->next;
		free(r->target);
		free(r);
		r = next;
	}
	free(cmd);
}

/*For each p->cmds[i] command, call free_exec_cmd.
Frees the p->cmds array.
Frees p.
We use when the builder fails in the middle.
When you're finished using the pipeline.*/
void	free_pipeline(t_pipeline *p)
{
	size_t	i;

	if (!p)
		return ;
	i = 0;
	while (i < p->count)
	{
		free_exec_cmd(p->cmds[i]);
		i++;
	}
	free(p->cmds);
	free(p);
}

