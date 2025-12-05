/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:31:15 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/03 17:31:18 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>

static int	count_argv(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

/*It's a helper to avoid always copying the same free while.*/
void	str_arr_free(char **arr, int used)
{
	int	i;

	if (!arr)
		return ;
	if (used < 0)
	{
		i = 0;
		while (arr[i])
		{
			free(arr[i]);
			i++;
		}
	}
	else
	{
		i = used - 1;
		while (i >= 0)
		{
			free(arr[i]);
			i--;
		}
	}
	free(arr);
}

/*Duplicate a NULL-terminated char ** array.
Calculate the length (count_argv),
Allocate len + 1 pointers,
Do ft_strdup each string.
If something fails, free what it did (str_arr_free(copy, i)).
Is useful because:
The parser has its own args (t_cmd->args),
The execution has its own argv (t_exec_cmd->argv).*/
char	**dup_argv(char **args)
{
	char	**copy;
	int		len;
	int		i;

	if (!args)
		return (NULL);
	len = count_argv(args);
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = ft_strdup(args[i]);
		if (!copy[i])
		{
			str_arr_free(copy, i);
			return (NULL);
		}
		i++;
	}
	copy[len] = NULL;
	return (copy);
}

/*It counts how many commands (t_cmd) are in the parser's linked list.
It tells us how many t_exec_cmd * 
are needed to allocate in the pipeline.
If we echo hello | grep c | wc -l, 
the t_cmd list will have 3 nodes*/
int	count_parsed_cmds(t_cmd *parsed)
{
	int	count;

	count = 0;
	while (parsed)
	{
		count++;
		parsed = parsed->next;
	}
	return (count);
}

