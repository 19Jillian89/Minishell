/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:29:58 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:45:47 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
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
    R_IN,       // <
    R_OUT,      // >
    R_APP,      // >>
    R_HEREDOC   // <<
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type     type;
    char            *target;    // filename o delimiter dell'heredoc
    struct s_redir  *next;
}   t_redir;

typedef struct s_exec_cmd
{
    char    **argv;     // argomenti del comando
    t_redir *redirs;    // lista di redirections
}   t_exec_cmd;

typedef struct s_pipeline
{
    t_exec_cmd  **cmds; // array di comandi
    size_t      count;  // quanti comandi nella pipeline
}   t_pipeline;

/* ************************************************************************** */
/*                               FUNCTIONS                                    */
/* ************************************************************************** */

/* Path handling */
char	*find_command(char *cmd, char **envp);

/* Esecuzione di un singolo comando esterno */
void    execute_single_cmd(t_exec_cmd *cmd, t_envc *envc);

/* Pipeline (multi comandi) */
void    exec_pipeline(t_pipeline *p, t_envc *envc);
void    exec_pipeline_child(t_pipeline *p, t_envc *envc,
                             int i, int prev_read, int pipefd[2]);
int     handle_input_fd(int prev_read);
int     handle_output_fd(int is_last, int pipefd[2]);
void    close_parent_fds(int prev_read, int pipefd[2], int is_last);
void    wait_pipeline(pid_t last_pid, t_envc *envc);

/* Redirections */
int     apply_redirections(t_redir *redirs, t_envc *envc);

/* run_pipeline: entry point dall'esterno (bridge) */
void    run_pipeline(t_pipeline *p, t_envc *envc);

/* ************************************************************************** */
/*                          PARSER → EXECUTION BRIDGE                         */
/* ************************************************************************** */

/* builder_utils.c */
void        str_arr_free(char **arr, int used);
char        **dup_argv(char **args);
int         count_parsed_cmds(t_cmd *parsed);

/* builder_redirs.c */
int         add_redir(t_redir **list, t_redir_type type, const char *target);

/* builder_free.c */
void        free_exec_cmd(t_exec_cmd *cmd);
void        free_pipeline(t_pipeline *p);

/* builder_cmd.c */
t_exec_cmd  *build_exec_cmd(t_cmd *p);
t_pipeline  *build_pipeline(t_cmd *parsed);

/* Heredoc functions */
int     handle_heredocs(t_pipeline *p, t_envc *envc);
char    *tmp_heredoc(t_envc *envc);
int     heredoc_quote(const char *sign);
char    *remove_quotes(const char *sign);
int     heredoc_child_process(const char *delim, const char *filename,
            t_envc *envc, int expand);
int     create_heredoc_for_redir(t_redir *redir, t_envc *envc);

#endif
