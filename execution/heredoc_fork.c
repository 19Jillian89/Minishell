/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:16:54 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:11:37 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

/*
** handle_child_status:
** --------------------------------------
** This function checks how the child process terminated.
**
** Case 1: Child terminated due to SIGINT (Ctrl+C)
**   -> Set exit_code = 130 (bash convention)
**   -> Remove the temporary heredoc file
**   -> Return -1 to report failure
**
** Case 2: Child exited normally but with a non-zero status
**   -> Set exit_code = 1
**   -> Remove temporary file
**   -> Return -1
**
** Case 3: Success
**   -> Return 0
*/
static int	handle_child_status(int status, char *filename, t_envc *envc)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		envc->exit_code = 130;
		unlink(filename);
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		envc->exit_code = 1;
		unlink(filename);
		return (-1);
	}
	return (0);
}

/*
** fork_heredoc:
** --------------------------------------
** Creates a child process to handle the heredoc input.
**
** Why fork?
**   Because reading heredoc must NOT block or break the parent shell.
**   Child reads lines, parent waits and handles signals cleanly.
**
** Child:
**   - Sets up child-specific signals
**   - Calls heredoc_child_process
**   - Exits with 0 or 1 depending on success
**
** Parent:
**   - Waits with waitpid()
**   - Passes the result to handle_child_status()
*/
static int	fork_heredoc(char *del, char *filename, t_envc *envc, int exp)
{
	pid_t	pid;
	int	status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork heredoc");
		return (-1);
	}
	if (pid == 0)
	{
		setup_signals_child();
		if (heredoc_child_process(del, filename, envc, exp) != 0)
			exit(1);
		exit(0);
	}
	waitpid(pid, &status, 0);
	return (handle_child_status(status, filename, envc));
}

/*
** cleanup_heredoc:
** --------------------------------------
** Utility function to free delimiter and filename
** in case of failure.
*/
static void	cleanup_heredoc(char *delim, char *filename)
{
	free(delim);
	free(filename);
}


/*
** create_heredoc_for_redir:
** --------------------------------------
** Main function used by the parser/executor when encountering <<REDIR.
**
** Steps:
**   1. Determine if expansion should happen
**      (no expansion if delimiter is quoted)
**
**   2. Remove quotes from the delimiter
**
**   3. Generate a temporary filename for storing heredoc content
**
**   4. Fork and process the heredoc
**
**   5. If successful:
**      - Replace redir->target with the filename
**      - Convert redirection type to R_IN (file input)
**
** Return:
**   0 on success
**  -1 on failure
*/
int	create_heredoc_for_redir(t_redir *redir, t_envc *envc)
{
	char	*filename;
	char	*delim;
	int		expand;

	expand = !heredoc_quote(redir->target);
	delim = remove_quotes(redir->target);
	if (!delim)
		return (-1);
	filename = tmp_heredoc(envc);
	if (!filename)
	{
		free(delim);
		return (-1);
	}
	if (fork_heredoc(delim, filename, envc, expand) == -1)
	{
		cleanup_heredoc(delim, filename);
		return (-1);
	}
	free(delim);
	free(redir->target);
	redir->target = filename;
	redir->type = R_IN;
	return (0);
}

/*
🇮🇹 Questo file gestisce il fork necessario per i heredoc (<<).

🔹 Perché serve il fork?
Perché il parent shell non deve essere influenzato da:
-lettura blocccante
-CTRL+C durante l'inserimento del heredoc
-segfault o errori del child
-Il child gestisce tutto → il parent aspetta.

🔹 Passi principali
create_heredoc_for_redir():
-controlla se il delimitatore è quotato (niente espansione)
-rimuove le virgolette
-crea un nome file temporaneo
-chiama fork_heredoc()
🔹fork_heredoc():
-fork del processo
-child: legge il heredoc e scrive nel file
-parent: aspetta e interpreta il risultato
-handle_child_status():
-interpreta come è morto il child (SIGINT → 130)
-cancella il file se fallito
-Aggiorna redir->target col file del heredoc e lo 
trasforma in un redirect in input.
*/
