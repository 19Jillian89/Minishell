/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utility.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 01:06:15 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/28 01:17:16 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static char **dup_env(char **envp)
{
        int     i;
        int     len;
        char    **copy;

        len = 0;
        while (envp && envp[len])
                len++;
        copy = malloc(sizeof(char *) * (len + 1));
        if (!copy)
                return (NULL);
        i = 0;
        while (i < len)
        {
                copy[i] = ft_strdup(envp[i]);
                if (!copy[i])
                {
                        while (i > 0)
                                free(copy[--i]);
                        free(copy);
                        return (NULL);
                }
                i++;
        }
        copy[len] = NULL;
        return (copy);
}

int init_envc(t_envc *envc, char **envp)
{
        envc->env = dup_env(envp);
        if (!envc->env && envp && envp[0])
                return (1);
        envc->exit_code = 0;
        envc->should_exit = 0;
        envc->heredoc_id = 0;
        return (0);
}

void free_envc(t_envc *envc)
{
        int i;

        if (!envc || !envc->env)
                return ;
        i = 0;
        while (envc->env[i])
        {
                free(envc->env[i]);
                i++;
        }
        free(envc->env);
        envc->env = NULL;
}

