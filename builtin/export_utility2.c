/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utility2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 23:36:10 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/26 00:39:13 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdlib.h>
#include <unistd.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static char     **copy_env(char **env)
{
        int     len;
        int     i;
        int     j;
        char    **copy;

        len = 0;
        while (env && env[len])
                len++;
        copy = malloc(sizeof(char *) * (len + 1));
        if (!copy)
                return (NULL);
        i = 0;
        while (i < len)
        {
                copy[i] = ft_strdup(env[i]);
                if (!copy[i])
                {
                        j = 0;
                        while (j < i)
                        {
                                free(copy[j]);
                                j++;
                        }
                        free(copy);
                        return (NULL);
                }
                i++;
        }
        copy[len] = NULL;
        return (copy);
}

static void     sort_env(char **env)
{
        int     i;
        int     j;
        char    *tmp;

        i = 0;
        while (env && env[i] && env[i + 1])
        {
                j = i + 1;
                while (env[j])
                {
                        if (ft_strcmp(env[i], env[j]) > 0)
                        {
                                tmp = env[i];
                                env[i] = env[j];
                                env[j] = tmp;
                        }
                        j++;
                }
                i++;
        }
}

static void     export_entry(char *entry)
{
        char    *eq_sign;

        eq_sign = ft_strchr(entry, '=');
        if (eq_sign)
        {
                write(1, "declare -x ", 11);
                write(1, entry, eq_sign - entry);
                write(1, "=\"", 2);
                write(1, eq_sign + 1, ft_strlen(eq_sign + 1));
                write(1, "\"\n", 2);
        }
        else
        {
                write(1, "declare -x ", 11);
                write(1, entry, ft_strlen(entry));
                write(1, "\n", 1);
        }
}

void    export_sorted(char **env)
{
        char    **sorted;
        int     i;

        sorted = copy_env(env);
        if (!sorted)
                return ;
        sort_env(sorted);
        i = 0;
        while (sorted[i])
        {
                export_entry(sorted[i]);
                free(sorted[i]);
                i++;
        }
        free(sorted);
}

