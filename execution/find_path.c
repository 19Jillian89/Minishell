/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:52:57 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/06 18:25:30 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Estrae le directory dal PATH dell'ambiente */
char	**extract_dir_paths(char **envp)
{
	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	if (!*envp)
		return (NULL);
	return (ft_split(*envp + 5, ':'));
}

/* Crea il percorso completo: prefix + "/" + cmd */
char	*create_path(char *cmd, char *prefix)
{
	char	*tmp;
	char	*full_path;

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

/* Libera un array di stringhe allocato dinamicamente */
void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/* Cerca il percorso eseguibile del comando (simile a shell PATH lookup) */
char	*find_command(char *cmd, char **envp)
{
	char	**dirs;
	char	*cmd_path;
	int		i;

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
			return (free_array(dirs), cmd_path);
		i++;
	}
	free_array(dirs);
	return (NULL);
}
