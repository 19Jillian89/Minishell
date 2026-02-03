/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:08:02 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 17:55:16 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>

/*This file contains helper utilities used by the heredoc system.
It handles:
Generating unique temporary filenames.
Detecting quoted delimiters.
Removing quotes from delimiters.
---------------------------------
tmp_heredoc()
Generates a unique temporary filename:
Converts envc->heredoc_id to string.
Builds name: .heredoc_<number>
Increments heredoc_id for the next heredoc.*/
char	*tmp_heredoc(t_envc *envc)
{
	char	*str;
	char	*name;

	str = ft_itoa(envc->heredoc_id);
	if (!str)
		return (NULL);
	envc->heredoc_id++;
	name = ft_strjoin(".heredoc_", str);
	free(str);
	return (name);
}

/*Checks whether the heredoc delimiter contains quotes.
Returns 1 if ' or " is found.
Returns 0 otherwise.
This information is later used to decide if variable expansion is allowed.*/
int	heredoc_quote(const char *sign)
{
	int	i;

	if (!sign)
		return (0);
	i = 0;
	while (sign[i])
	{
		if (sign[i] == '\'' || sign[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

static int	copy_char(char c)
{
	if (c == '\'' || c == '\"')
		return (0);
	return (1);
}

char	*remove_quotes(const char *sign)
{
	int		i;
	int		j;
	char	*del;

	if (!sign)
		return (NULL);
	del = malloc(ft_strlen(sign) + 1);
	if (!del)
		return (NULL);
	i = 0;
	j = 0;
	while (sign[i])
	{
		if (copy_char(sign[i]))
		{
			del[j] = sign[i];
			j++;
		}
		i++;
	}
	del[j] = '\0';
	return (del);
}
