/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utility.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:03:46 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/25 23:13:58 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/*static int	env_len(char **env)
{
	int	len;

	len = 0;
	while (env && env[len])
		len++;
	return (len);
}*/

static char	**var_add(char **env, const char *str)
{
	int	i;
	int	len;
	char	**new_env;

	len = 0;
	while (env && env[len])
		len++;
	new_env = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (env);
	i = 0;
	while (i < len)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[len] = ft_strdup(str);
	if (!new_env[len])
	{
		free(new_env);
		return (env);
	}
	new_env[len + 1] = NULL;
	free(env);
	return (new_env);
}

static int      find_index(char **env, const char *entry)
{
        int     i;
        size_t  len_value;

        len_value = 0;
        while (entry[len_value] && entry[len_value] != '=')
                len_value++;
        i = 0;
        while (env && env[i])
        {
                if (ft_strncmp(env[i], entry, len_value) == 0
                        && (env[i][len_value] == '=' || env[i][len_value] == '\0'))
                        return (i);
                i++;
        }
        return (-1);
}

char    **set_var(char **env, const char *entry)
{
        int     idx;
        char    *dup;

        idx = find_index(env, entry);
        if (idx >= 0)
        {
                dup = ft_strdup(entry);
                if (!dup)
                        return (env);
                free(env[idx]);
                env[idx] = dup;
                return (env);
        }
        return (var_add(env, entry));
}
