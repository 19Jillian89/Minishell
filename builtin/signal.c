/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:01:20 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/26 01:12:43 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	g_signal = 0;

/*
** Handler per SIGINT mentre la shell è nel prompt (readline).
** Deve essere async-safe → NIENTE printf, niente malloc, niente libft.
*/
void    sigint_handler(int sig)
{
    (void)sig;
    g_signal = SIGINT;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

/*
** Stato segnali durante il prompt interattivo (Ctrl-C, Ctrl-\).
*/
void    setup_signals_interactive(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}

/*
** Stato segnali nei processi figli (comandi eseguiti con fork).
*/
void    setup_signals_child(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

/*
** Stato segnali nel padre mentre aspetta i figli.
** La shell NON deve reagire ai segnali durante l'esecuzione.
*/
void    setup_signals_parent_exec(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}
