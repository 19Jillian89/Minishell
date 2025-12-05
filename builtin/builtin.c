/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 18:40:20 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:22:16 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"
#include "builtin.h"
#include <stddef.h>
#include <string.h>

/*
** get_env_value:
** -------------------------------------------------------
** Searches through the provided environment array `env`
** for a variable matching the given key (e.g., "HOME").
**
** It checks each entry of the form "KEY=value" and if the
** prefix matches `key`, returns a pointer *after* the '='.
**
** Example:
**   env: ["PATH=/bin:/usr/bin", "HOME=/users/..."]
**   key: "HOME"
**   return → "/users/..."
**
** Return:
**   - pointer to the value string
**   - NULL if key not found or invalid input
*/
char *get_env_value(char **env, char *key)
{
    int i;
    size_t key_len;

    if (!env || !key)
        return NULL;

    key_len = strlen(key);
    i = 0;
    while (env[i])
    {
        if (strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
            return env[i] + key_len + 1;
        i++;
    }
    return NULL;
}

/*
** is_builtin:
** -------------------------------------------------------
** Checks whether the given command name corresponds
** to one of the supported builtin commands.
**
** Builtins do NOT require fork to execute when they
** need to modify the shell state (cd, export, unset...).
**
** Return:
**   1 if builtin, 0 otherwise
*/
int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    if (ft_strcmp(cmd, "cd") == 0)
        return 1;
    if (ft_strcmp(cmd, "echo") == 0)
        return 1;
    if (ft_strcmp(cmd, "pwd") == 0)
        return 1;
    if (ft_strcmp(cmd, "export") == 0)
        return 1;
    if (ft_strcmp(cmd, "unset") == 0)
        return 1;
    if (ft_strcmp(cmd, "env") == 0)
        return 1;
    if (ft_strcmp(cmd, "exit") == 0)
        return 1;
    return 0;
}

/*
** execute_builtin:
** -------------------------------------------------------
** Executes the correct builtin function based on cmd->argv[0].
**
** Notes:
** - Builtins are executed *in the parent* when needed,
**   so they can modify environment, working directory, etc.
**
** - The return value of each builtin is stored in envc->exit_code,
**   following bash behavior.
**
** Return:
**   The exit status returned by the builtin function.
**   If the command is not a builtin, returns 0.
*/
int execute_builtin(t_exec_cmd *cmd, t_envc *envc)
{
    int status;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return 0;

    if (ft_strcmp(cmd->argv[0], "cd") == 0)
        status = cd_builtin(cmd->argv, envc);
    else if (ft_strcmp(cmd->argv[0], "echo") == 0)
        status = echo_builtin(cmd->argv);
    else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
        status = pwd_builtin();
    else if (ft_strcmp(cmd->argv[0], "export") == 0)
        status = export_builtin(cmd, envc);
    else if (ft_strcmp(cmd->argv[0], "unset") == 0)
        status = unset_builtin(cmd, envc);
    else if (ft_strcmp(cmd->argv[0], "env") == 0)
        status = env_builtin(cmd, envc);
    else if (ft_strcmp(cmd->argv[0], "exit") == 0)
        status = exit_builtin(cmd->argv, envc);
    else
        return 0;

    envc->exit_code = status;
    return status;
}

/*
🇮🇹
get_env_value
-Cerca all’interno dell’array di ambiente env una variabile che corrisponda 
alla chiave fornita (es. “HOME”).
-Controlla ogni voce nel formato "CHIAVE=valore" e, se il prefisso coincide con key, 
restituisce un puntatore alla parte subito dopo il simbolo '='.

Esempio:
env: ["PATH=/bin:/usr/bin", "HOME=/users/..."]
key: "HOME"
return → "/users/..."

Ritorno:
-un puntatore alla stringa del valore
-NULL se la chiave non è trovata o se gli input sono invalidi

is_builtin
Controlla se il nome del comando fornito corrisponde a uno dei comandi builtin supportati.
I builtin non richiedono fork quando devono modificare lo stato della shell (cd, export, unset...).

Ritorno:
-1 se è un builtin
-0 altrimenti

execute_builtin
Esegue la funzione builtin corretta basandosi su cmd->argv[0].

Note:
-I builtin vengono eseguiti nel processo padre quando necessario, così da poter modificare ambiente, 
directory corrente, ecc.
-Il valore di ritorno di ogni builtin viene salvato in envc->exit_code, 
seguendo il comportamento di bash.

Ritorno:
-Lo stato di uscita restituito dal builtin
-0 se il comando non è un builtin
*/
