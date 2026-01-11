/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:19:10 by tvithara          #+#    #+#             */
/*   Updated: 2026/01/11 17:05:26 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_var_name(char *str, int i);
/*
** Expand variable in unquoted context
** Returns new position in string
*/
int	expand_variable(char *str, int i, t_expand_args *expand_args)
{
	i++;
	if (str[i] == '?')
	{
		expand_args->result = ft_strjoin_free(expand_args->result,
				ft_itoa(expand_args->exit_status));
		return (i + 1);
	}
	else if (ft_isalpha(str[i]) || str[i] == '_')
		return (expand_env_variable(str, i, expand_args));
	else
	{
		expand_args->result = ft_strjoin_free(expand_args->result,
				ft_substr(str, i - 1, 1));
		return (i);
	}
}

int	expand_env_variable(char *str, int i, t_expand_args *expand_args)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	var_name = extract_var_name(str, i);
	if (var_name == NULL)
		return (i);
	var_value = getenv(var_name);
	if (var_value != NULL)
	{
		expand_args->result = ft_strjoin_free(expand_args->result,
				ft_strdup(var_value));
	}
	var_len = ft_strlen(var_name);
	free(var_name);
	return (i + var_len);
}

static char	*extract_var_name(char *str, int i)
{
	int		start;
	int		len;
	char	*var_name;

	start = i;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	len = i - start;
	var_name = ft_substr(str, start, len);
	return (var_name);
}
