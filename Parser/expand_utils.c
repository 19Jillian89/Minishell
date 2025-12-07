/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:19:10 by tvithara          #+#    #+#             */
/*   Updated: 2025/12/07 15:19:14 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
