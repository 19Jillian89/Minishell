/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:52:57 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 18:48:42 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

/*
** extract_dir_paths
** ------------------
** Extracts the list of directories inside the PATH environment variable.
**
** @envp: environment variables array.
**
** Searches for the "PATH=" entry inside envp, skips the prefix, and splits the
** directory list by ':' into an array of strings. Returns NULL if PATH is not
** found.
**
** Return: array of directory strings, or NULL if PATH is missing.
*/
static char     **extract_dir_paths(char **envp)
{
        char    *path_var;

        while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
                envp++;
        if (!*envp)
                return (NULL);
        path_var = *envp + 5;
        return (ft_split(path_var, ':'));
}

/*
** create_path
** ------------
** Builds a full executable path by joining a prefix directory with a command.
**
** @cmd: command name (e.g. "ls").
** @prefix: one directory from PATH (e.g. "/usr/bin").
**
** Creates prefix + "/" + cmd, checks if the resulting path is executable.
** Returns a newly allocated string if valid, NULL otherwise.
**
** Return: full path string if executable exists, NULL otherwise.
*/
static char     *create_path(char *cmd, char *prefix)
{
        char    *tmp;
        char    *full_path;

        tmp = ft_strjoin(prefix, "/");
        if (!tmp)
                return (NULL);
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (!full_path)
                return (NULL);
        if (access(full_path, X_OK) == 0)
                return (full_path);
        free(full_path);
        return (NULL);
}

/*
** free_array
** -----------
** Frees a NULL-terminated array of strings.
**
** @arr: array to free.
**
** Safely frees each string and then the array itself.
*/
static void     free_array(char **arr)
{
        int     i;

        if (!arr)
                return ;
        i = 0;
        while (arr[i])
        {
                free(arr[i]);
                i++;
        }
        free(arr);
}

/*
** find_command
** -------------
** Resolves a command name into an executable path.
**
** @cmd: the command to locate.
** @envp: environment variables array.
**
** If cmd starts with '/' or '.', it is treated as an absolute or relative path:
**   - if it points to an executable → returns a copy of cmd
**   - otherwise → returns NULL
**
** Otherwise, the function searches each directory in PATH to find an executable
** file matching the command.
**
** Return: newly allocated full path, or NULL if not found.
*/
char    *find_command(char *cmd, char **envp)
{
        char    **dirs;
        char    *cmd_path;
        int     i;

        if (!cmd || cmd[0] == '\0')
                return (NULL);
        if (cmd[0] == '/' || cmd[0] == '.')
        {
                if (access(cmd, X_OK) == 0)
                        return (ft_strdup(cmd));
                return (NULL);
        }
        dirs = extract_dir_paths(envp);
        if (!dirs)
                return (NULL);
        i = 0;
        while (dirs[i])
        {
                cmd_path = create_path(cmd, dirs[i]);
                if (cmd_path)
                {
                        free_array(dirs);
                        return (cmd_path);
                }
                i++;
        }
        free_array(dirs);
        return (NULL);
}
