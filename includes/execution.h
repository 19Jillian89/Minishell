/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:29:58 by ilnassi           #+#    #+#             */
/*   Updated: 2026/01/30 16:48:46 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/includes/libft.h"
# include "minishell.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>

/* ************************************************************************** */
/*                               STRUCTURES                                   */
/* ************************************************************************** */

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APP,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*target;
	struct s_redir		*next;
}	t_redir;

typedef struct s_exec_cmd
{
	char	**argv;
	t_redir	*redirs;
}	t_exec_cmd;

typedef struct s_pipe_status
{
	int		prev_read;
	int		pipefd[2];
	pid_t	last_pid;
}	t_pipe_status;

typedef struct s_pipeline
{
	t_exec_cmd	**cmds;
	size_t		count;
}	t_pipeline;

/* ************************************************************************** */
/*                               FUNCTIONS                                    */
/* ************************************************************************** */

/* Path handling */
char		*find_command(char *cmd, char **envp);

/* Single command execution */
void		execute_single_cmd(t_exec_cmd *cmd, t_shell *shell);
void		exec_external_child(t_exec_cmd *cmd, t_shell *shell);

/* Pipeline */
void		exec_pipeline(t_pipeline *p, t_shell *shell);
void		exec_pipeline_child(t_pipeline *p, t_shell *shell, int i,
				t_pipe_status *status);
int			handle_input_fd(int prev_read);
int			handle_output_fd(int is_last, int pipefd[2]);
void		close_parent_fds(int prev_read, int pipefd[2], int is_last);
void		wait_pipeline(pid_t last_pid, t_envc *envc);
void		prepare_child_fds(t_pipeline *p, int i, t_pipe_status *status);

/* Redirections */
int			apply_redirections(t_redir *redirs, t_shell *shell);

/* Bridge */
void		run_pipeline(t_pipeline *p, t_shell *shell);

/* ************************************************************************** */
/*                          PARSER → EXECUTION BRIDGE                         */
/* ************************************************************************** */

/* builder_utils.c */
void		str_arr_free(char **arr, int used);
char		**dup_argv(char **args);
int			count_parsed_cmds(t_cmd *parsed);

/* builder_redirs.c */
int			add_redir(t_redir **list, t_redir_type type, const char *target);

/* builder_free.c */
void		free_exec_cmd(t_exec_cmd *cmd);
void		free_pipeline(t_pipeline *p);

/* builder_cmd.c */
t_exec_cmd	*build_exec_cmd(t_cmd *p);
t_pipeline	*build_pipeline(t_cmd *parsed);

/* Heredoc */
int			handle_heredocs(t_pipeline *p, t_envc *envc);
char		*tmp_heredoc(t_envc *envc);
int			heredoc_quote(const char *sign);
char		*remove_quotes(const char *sign);
int			heredoc_child_process(const char *delim, const char *filename,
				t_envc *envc, int expand);
int			create_heredoc_for_redir(t_redir *redir, t_envc *envc);

#endif
