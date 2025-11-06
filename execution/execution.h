/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:29:58 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/06 18:34:09 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <signal.h>

/* ************************************************************************** */
/*                               STRUCTURES                                   */
/* ************************************************************************** */

typedef struct s_cmd
{
	char	**argv;
	char	*path;
	int		in_fd;
	int		out_fd;
	pid_t	pid;
}	t_cmd;

typedef struct s_exec
{
	char	**envp;
	char	**paths;
	int		pipefd[2];
}	t_exec;

/* ************************************************************************** */
/*                               FUNCTIONS                                    */
/* ************************************************************************** */

/* Path handling */
char	**extract_dir_paths(char **envp);
char	*find_command(char *cmd, char **envp);
void	free_array(char **arr);

/* Execution */
void	execute_single_cmd(t_cmd *cmd, t_exec *exec);
void	execute_pipeline(t_cmd **cmds, int cmd_count, t_exec *exec);
void	child_process(t_cmd *cmd, t_exec *exec, int is_first, int is_last);

/* Error management */
void	error_exit(char *msg);

#endif
