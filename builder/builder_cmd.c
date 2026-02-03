/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:32:47 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:30:18 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>

/*
copy_redirs:
------------------------------------------------------
Copia TUTTE le redirections dal parsed command (t_cmd)
alla struttura di exec (t_exec_cmd), mantenendo l'ordine.
Return:
- 1 on success (o se src == NULL)
- 0 se add_redir() fallisce
*/
static int	copy_redirs(t_exec_cmd *dst, t_redir *src)
{
	while (src)
	{
		if (!add_redir(&dst->redirs, src->type, src->target))
			return (0);
		src = src->next;
	}
	return (1);
}

/*
build_exec_cmd:
-------------------------------------------------------
Converts a parsed t_cmd node into a fully allocated
t_exec_cmd structure ready for execution.
Steps:
1) Allocate t_exec_cmd
2) Duplicate argument vector (dup_argv)
3) Initialize redirections list
4) Insert input/output redirections if present
If any step fails, all allocated memory is cleaned
using free_exec_cmd(), and NULL is returned.
Return:
- Pointer to a valid t_exec_cmd on success
- NULL on failure
*/
t_exec_cmd	*build_exec_cmd(t_cmd *p)
{
	t_exec_cmd	*command;

	command = malloc(sizeof(t_exec_cmd));
	if (!command)
		return (NULL);
	command->argv = dup_argv(p->args);
	command->redirs = NULL;
	if (!command->argv)
	{
		free(command);
		return (NULL);
	}
	if (!copy_redirs(command, p->redirs))
	{
		free_exec_cmd(command);
		return (NULL);
	}
	return (command);
}

static int	pipeline_fill(t_pipeline *p, t_cmd *parsed)
{
	size_t	i;

	i = 0;
	while (i < p->count)
	{
		p->cmds[i] = build_exec_cmd(parsed);
		if (!p->cmds[i])
		{
			p->count = i;
			free_pipeline(p);
			return (0);
		}
		parsed = parsed->next;
		i++;
	}
	return (1);
}

/*
pipeline_fill:
------------------------------------------------------
Fills a t_pipeline structure with a list of t_exec_cmd
built from the linked list of parsed commands.
Behavior:
- Iterates through the pipeline array
- Calls build_exec_cmd() for each parsed node
- On failure:
→ Frees all already created commands
→ Sets p->count to the number of valid commands
→ Frees the pipeline and returns 0
Return:
- 1 on full success
- 0 if any command fails to build
*/
t_pipeline	*build_pipeline(t_cmd *parsed)
{
	t_pipeline	*p;
	int			count;

	count = count_parsed_cmds(parsed);
	if (count <= 0)
		return (NULL);
	p = malloc(sizeof(t_pipeline));
	if (!p)
		return (NULL);
	p->cmds = malloc(sizeof(t_exec_cmd *) * count);
	if (!p->cmds)
	{
		free(p);
		return (NULL);
	}
	p->count = count;
	if (!pipeline_fill(p, parsed))
		return (NULL);
	return (p);
}
