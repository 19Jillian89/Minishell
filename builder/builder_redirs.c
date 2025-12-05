/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder_redirs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:31:38 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/03 17:31:52 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>

/*Allocates the node,
sets node->type,
runs ft_strdup(target) if target is not NULL,
if an error occurs in ft_strdup, frees the node and returns NULL.*/
static t_redir	*new_redir(t_redir_type type, const char *target)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->type = type;
	if (target)
	{
		node->target = ft_strdup(target);
		if (!node->target)
		{
			free(node);
			return (NULL);
		}
	}
	else
		node->target = NULL;
	node->next = NULL;
	return (node);
}

/*Creates a new t_redir node,
appends it to the linked redirection list.
If new_redir fails, returns 0 (error),
otherwise, appends it to the list and returns 1 (ok).
The t_redir list will then be read by the executor 
to open files, pipes, etc.*/
int	add_redir(t_redir **list, t_redir_type type, const char *target)
{
	t_redir	*node;
	t_redir	*tmp;

	node = new_redir(type, target);
	if (!node)
		return (0);
	if (!*list)
	{
		*list = node;
		return (1);
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
	return (1);
}

