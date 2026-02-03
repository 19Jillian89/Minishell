/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:18:44 by tvithara          #+#    #+#             */
/*   Updated: 2026/01/30 20:18:55 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h" 

static void	free_redirs(t_redir *r);

/*
** Skip over a quoted region without processing the content
*/
int	skip_quoted_region(char *input, int i)
{
	char	quote_type;

	quote_type = input[i];
	i++;
	while (input[i] != '\0' && input[i] != quote_type)
		i++;
	if (input[i] == quote_type)
		i++;
	return (i);
}

/*
** Free the linked list of command structures
*/
void	free_commands(t_cmd *commands)
{
	t_cmd	*temp;
	int		i;

	while (commands != NULL)
	{
		temp = commands;
		commands = commands->next;
		if (temp->args != NULL)
		{
			i = 0;
			while (temp->args[i] != NULL)
				free(temp->args[i++]);
			free(temp->args);
		}
		if (temp->redirs != NULL)
			free_redirs(temp->redirs);	
		if (temp->input_file != NULL)
			free(temp->input_file);
		if (temp->output_file != NULL)
			free(temp->output_file);
		if (temp->heredoc_delim != NULL)
			free(temp->heredoc_delim);
		free(temp);
	}
}

/*
** Extract variable name from string (alphanumeric + underscore)
*/
char	*extract_var_name(char *str, int i)
{
	int	start;
	int	len;

	start = i;
	len = 0;
	while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		i++;
		len++;
	}
	return (ft_substr(str, start, len));
}

/*
** Add a command to the command list
** Similar to adding a node to a linked list
*/
void	add_command(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (new_cmd == NULL)
		return ;
	if (*cmd_list == NULL)
	{
		*cmd_list = new_cmd;
	}
	else
	{
		current = *cmd_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_cmd;
	}
}

static void	free_redirs(t_redir *r)
{
	t_redir	*tmp;

	while (r)
	{
		tmp = r;
		r = r->next;
		if (tmp->target)
			free(tmp->target);
		free(tmp);
	}
}
