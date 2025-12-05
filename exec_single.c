/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 00:30:33 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 18:53:59 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

/*
** exec_external_child
** --------------------
** Executes a single external command inside the child process.
**
** @cmd: structure containing the command arguments and redirections.
** @envc: environment and exit-code structure.
**
** Sets up child signal handlers, applies redirections, resolves the command
** path via PATH lookups, and finally calls execve(). If the command cannot
** be found or executed, prints an appropriate error message and exits with
** the corresponding exit code (126 or 127).
**
** This function never returns: the child ends by execve() or exit().
*/
static void exec_external_child(t_exec_cmd *cmd, t_envc *envc)
{
    char    *path;

    setup_signals_child();
    if (apply_redirections(cmd->redirs, envc) != 0)
        exit(envc->exit_code);
    if (!cmd->argv || !cmd->argv[0])
        exit(0);
    path = find_command(cmd->argv[0], envc->env);
    if (!path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->argv[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        envc->exit_code = 127;
        exit(envc->exit_code);
    }
    execve(path, cmd->argv, envc->env);
    perror("minishell: execve");
    free(path);
    if (errno == EACCES)
        envc->exit_code = 126;
    else
        envc->exit_code = 127;
    exit(envc->exit_code);
}

/*
** update_exit_status_from_wait
** -----------------------------
** Interprets the exit status returned by waitpid() and updates envc->exit_code.
**
** @status: status returned by waitpid().
** @envc: environment and exit-code structure.
**
** - If the child exited normally, sets exit_code to its exit value.
** - If terminated by a signal, sets exit_code according to shell conventions:
**     SIGINT  → 130
**     SIGQUIT → 131 + prints "Quit (core dumped)"
**     Other   → 128 + signal number
*/
static void  update_exit_status_from_wait(int status, t_envc *envc)
{
    if (WIFEXITED(status))
        envc->exit_code = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT)
            envc->exit_code = 130;
        else if (WTERMSIG(status) == SIGQUIT)
        {
            envc->exit_code = 131;
            ft_putstr_fd("Quit (core dumped)\n", 2);
        }
        else
            envc->exit_code = 128 + WTERMSIG(status);
    }
}

/*
** execute_single_cmd
** -------------------
** Executes a command that is NOT part of a pipeline (a standalone command).
**
** @cmd: structure containing command arguments and possible redirections.
** @envc: environment and exit-code structure.
**
** Forks a child process to execute the command, applies correct signal
** behavior, waits for termination, and updates the exit code accordingly.
** Handles fork() and waitpid() errors gracefully.
*/
void    execute_single_cmd(t_exec_cmd *cmd, t_envc *envc)
{
    pid_t   pid;
    int     status;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return ;
    setup_signals_parent_exec();
    pid = fork();
    if (pid < 0)
    {
        perror("minishell: fork");
        envc->exit_code = 1;
        setup_signals_interactive();
        return ;
    }
    if (pid == 0)
        exec_external_child(cmd, envc);
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("minishell: waitpid");
        envc->exit_code = 1;
    }
    else
        update_exit_status_from_wait(status, envc);
    setup_signals_interactive();
}

/*
🇮🇹🔹 exec_external_child

È la funzione eseguita nel processo figlio quando devi lanciare un comando esterno (es. ls, cat, ecc.).
Fa queste cose:
-imposta i segnali per il child (Ctrl+C, Ctrl+)
-applica le redirezioni
-cerca il comando nel PATH (find_command)
-se non esiste → stampa “command not found” e termina
-se esiste → chiama execve()
-se execve fallisce → stampa errore e termina con codice 126/127
-Il child non ritorna mai alla shell: o esegue il comando, o muore.

🇮🇹🔹 update_exit_status_from_wait

Interpreta il codice di uscita restituito da waitpid():
-se il comando è uscito normalmente → prende il suo exit code
-se è stato ucciso da un segnale:
-SIGINT → 130
-SIGQUIT → 131 (+ stampa “Quit (core dumped)”)
-altri segnali → 128 + numero del segnale
Fa esattamente quello che fanno Bash e Zsh.

🇮🇹🔹 execute_single_cmd

Questa funzione serve quando il comando non fa parte di una pipeline.
Esegue il flusso classico:
-imposta segnali per il processo padre
-fork()
-nel child → chiama exec_external_child
-nel parent → aspetta il child con waitpid()
-aggiorna exit_code con update_exit_status_from_wait
-ripristina i segnali interattivi
-Gestisce correttamente errori di fork e waitpid.
*/
