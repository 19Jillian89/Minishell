/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:31:24 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/04 16:31:27 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"

/*Iterates through all redirections of a single command.

If a redirection is of type R_HEREDOC:
Calls create_heredoc_for_redir() to generate the temporary heredoc file.
If any heredoc creation fails:
Returns -1 to signal an error.

This function isolates heredoc handling at the command level.*/
static int	process_cmd_redirs(t_exec_cmd *cmd, t_envc *envc)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == R_HEREDOC)
		{
			if (create_heredoc_for_redir(redir, envc) != 0)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

/*Iterates through every command inside a pipeline.

For each command:
Calls process_cmd_redirs()
Stops immediately if an error occurs.

This ensures that all heredocs are fully 
prepared before execution begins.*/
int	handle_heredocs(t_pipeline *p, t_envc *envc)
{
	int		i;

	if (!p)
		return (0);
	i = 0;
	while (i < (int)p->count)
	{
		if (process_cmd_redirs(p->cmds[i], envc) == -1)
			return (-1);
		i++;
	}
	return (0);
}

/*🇮🇹Questo file coordina la creazione degli heredoc 
all’interno di una pipeline.
Il suo compito è scorrere tutti i comandi e le loro redirezioni, 
individuare le redirezioni di tipo heredoc (<<) e 
avviare il processo di creazione del file temporaneo.

process_cmd_redirs()
Scorre tutte le redirezioni di un singolo comando.
Se trova una redirezione di tipo R_HEREDOC:
Chiama create_heredoc_for_redir()
Se una creazione fallisce:
Ritorna -1
In pratica gestisce gli heredoc a livello di singolo comando.

handle_heredocs()
Scorre tutti i comandi della pipeline.
Per ogni comando:
Chiama process_cmd_redirs()
Si ferma subito in caso di errore.
Garantisce che tutti gli heredoc siano pronti prima dell’esecuzione.

Perché serve questo passaggio
Gli heredoc vanno preparati prima di eseguire i comandi perché:
Richiedono input utente.
Possono essere interrotti da segnali.
Producono file temporanei usati poi come input.*/
