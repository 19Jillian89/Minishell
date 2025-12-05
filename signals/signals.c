/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:01:20 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:38:32 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	g_signal = 0;

/*
** sigint_handler:
** -------------------------------------------------------
** Custom handler for SIGINT while the shell is idle in the
** interactive prompt (readline).
**
** Notes:
** - Must be async-signal-safe → no printf, malloc, free, or
**   any non-reentrant function.
** - readline provides specific helper functions that are 
**   safe to use inside signal handlers.
**
** Behavior:
** - Set global flag to notify shell of SIGINT
** - Print a newline to move to a fresh prompt
** - Clear the current input line
** - Redisplay a new prompt cleanly
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
** setup_signals_interactive:
** -------------------------------------------------------
** Configures signal behavior while the shell is waiting
** for user input in interactive mode.
**
** Behavior:
** - SIGINT  → handled by sigint_handler (Ctrl-C resets prompt)
** - SIGQUIT → ignored (Ctrl-\ does nothing)
**
** This matches the behavior of bash.
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
** setup_signals_child:
** -------------------------------------------------------
** Restores default signal behavior inside forked child
** processes running external commands.
**
** Behavior:
** - SIGINT  → default (terminates the process)
** - SIGQUIT → default (generates "Quit (core dumped)" if needed)
**
** This ensures child processes behave like normal programs.
*/
void    setup_signals_child(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

/*
** setup_signals_parent_exec:
** -------------------------------------------------------
** Configures signal behavior in the parent *while commands
** are running*, i.e., after forking and before waiting.
**
** Behavior:
** - SIGINT  → ignored
** - SIGQUIT → ignored
**
** The parent shell must NOT react to Ctrl-C/Ctrl-\ during
** execution—the children handle them instead.
*/
void    setup_signals_parent_exec(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

/*
🌀​sigint_handler
Questo è il gestore personalizzato per SIGINT quando la shell è nel prompt, cioè durante la readline.
Cosa fa:
-imposta una variabile globale (g_signal) per notificare il main loop;
-scrive un \n per andare a capo;
dice a readline di:
-cancellare la linea attuale
-mostrare una nuova linea pulita
-ridisegnare il prompt

Importantissimo: tutto quello che fai in un signal handler deve essere async-signal-safe, quindi:
❌ niente printf
❌ niente malloc / free
❌ niente funzioni libft
✔ write() è sicura
✔ rl_replace_line è pensata proprio per questo

🌀​setup_signals_interactive
Imposta il comportamento dei segnali quando la shell aspetta input dell’utente (interattiva).
SIGINT (Ctrl-C) → gestito da sigint_handler
Comportamento identico a bash: cancella linea e mostra un nuovo prompt.
SIGQUIT (Ctrl-) → ignorato
Così Ctrl-\ non chiude la shell.
È lo stato dei segnali “normale della minishell”.

🌀​setup_signals_child
Impostato nei processi figli dopo fork().
SIGINT → default
SIGQUIT → default
Serve perché i comandi lanciati dalla shell devono comportarsi come normali programmi Linux.

Esempio:
se lanci cat e premi Ctrl-C, deve terminare il processo figlio, non la shell.

🌀​setup_signals_parent_exec
Usato dal padre mentre aspetta la terminazione dei figli (dopo fork, prima di waitpid).
Durante l’esecuzione dei comandi:
-La shell non deve reagire ai segnali → i figli reagiscono al posto suo.

Quindi:
SIGINT → ignorato
SIGQUIT → ignorato

Esempio:
se scrivi sleep 10 e premi Ctrl-C:
il figlio viene terminato, la shell NO.
*/
