/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 23:14:31 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/26 00:41:48 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/* Controlla che la parte prima di '=' sia un identificatore valido */
static int  valid_identifier(const char *str)
{
        int     i;

        if (!str || (!ft_isalpha((unsigned char)str[0]) && str[0] != '_'))
                return (0);
        i = 1;
        while (str[i] && str[i] != '=')
        {
            if (!ft_isalnum((unsigned char)str[i]) && str[i] != '_')
                    return (0);
            i++;
        }
        return (1);
}

int export_builtin(t_exec_cmd *cmd, t_envc *envc)
{
        int     i;
        int     status;

        if (!cmd || !cmd->argv || !envc)
                return (0);
        if (!cmd->argv[1])
        {
		export_sorted(envc->env);
                envc->exit_code = 0;
                return (0);
        }
        i = 1;
        status = 0;
        while (cmd->argv[i])
        {
                if (!valid_identifier(cmd->argv[i]))
                {
                        ft_putstr_fd("minishell: export: `", 2);
                        ft_putstr_fd(cmd->argv[i], 2);
                        ft_putstr_fd("': not a valid identifier\n", 2);
                        status = 1;
                }
                else if (ft_strchr(cmd->argv[i], '='))
                        envc->env = set_var(envc->env, cmd->argv[i]);
                i++;
        }
        envc->exit_code = status;
        return (status);
}
