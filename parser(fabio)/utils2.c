/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:18:44 by tvithara          #+#    #+#             */
/*   Updated: 2025/12/07 15:18:47 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

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
** Get environment variable value
*/
/*char	*get_env_value(char *var_name, char **env)
{
	int		i;
	int		len;
	char	*env_var;

	i = 0;
	len = ft_strlen(var_name);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
		{
			env_var = ft_strdup(env[i] + len + 1);
			return (env_var);
		}
		i++;
	}
	return (NULL);
}
*/

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

void add_command(t_cmd **list, t_cmd *new_cmd)
{
    t_cmd  *temp;

    if (*list == NULL)
    {
        *list = new_cmd;
        return ;
    }
    temp = *list;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = new_cmd;
}

/*int expand_variable(char *str, int i, char **result, char **env, int exit_status)
{
    char *var_name;
    char *var_value;

    i++; // Skip $

    if (str[i] == '?')
    {
        var_value = ft_itoa(exit_status);
        *result = ft_strjoin_free(*result, var_value);
        return (i + 1);
    }
    else if (ft_isalpha(str[i]) || str[i] == '_')
    {
        var_name = extract_var_name(str, i);
        var_value = get_env_value(env, var_name);  // notare ordine dei parametri
        if (var_value)
            *result = ft_strjoin_free(*result, ft_strdup(var_value));
        i += ft_strlen(var_name);
        free(var_name);
        return (i);
    }
    else
    {
        // Not a valid variable, keep as literal $
        *result = ft_strjoin_free(*result, ft_substr(str, i - 1, 1));
        return (i);
    }
}
*/
