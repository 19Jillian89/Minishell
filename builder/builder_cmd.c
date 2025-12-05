/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builder_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:32:47 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:30:18 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include <stdlib.h>

/*
** set_input:
** -------------------------------------------------------
** Converts the parser's 'input_file' field into an R_IN
** redirection in the given t_exec_cmd structure.
**
** Behavior:
** - If p->input_file is NULL → nothing is added.
** - Otherwise → add_redir(&c->redirs, R_IN, p->input_file).
**
** Return:
** - 1 on success (or when no input file is present)
** - 0 if add_redir() fails
*/
static int	set_input(t_exec_cmd *c, t_cmd *p)
{
	if (!p->input_file)
		return (1);
	if (!add_redir(&c->redirs, R_IN, p->input_file))
		return (0);
	return (1);
}

/*
** set_output:
** -------------------------------------------------------
** Converts the parser's 'output_file' field into either
** a R_OUT (truncate) or R_APP (append) redirection.
**
** Behavior:
** - If p->output_file is NULL → nothing is added.
** - Otherwise:
**       append_mode == 1 → R_APP
**       append_mode == 0 → R_OUT
**
** Return:
** - 1 on success (or when no output file is present)
** - 0 if add_redir() fails
*/
static int	set_output(t_exec_cmd *c, t_cmd *p)
{
	t_redir_type	type;

	if (!p->output_file)
		return (1);
	if (p->append_mode)
		type = R_APP;
	else
		type = R_OUT;
	if (!add_redir(&c->redirs, type, p->output_file))
		return (0);
	return (1);
}

/*
** build_exec_cmd:
** -------------------------------------------------------
** Converts a parsed t_cmd node into a fully allocated
** t_exec_cmd structure ready for execution.
**
** Steps:
** 1) Allocate t_exec_cmd
** 2) Duplicate argument vector (dup_argv)
** 3) Initialize redirections list
** 4) Insert input/output redirections if present
**
** If any step fails, all allocated memory is cleaned
** using free_exec_cmd(), and NULL is returned.
**
** Return:
** - Pointer to a valid t_exec_cmd on success
** - NULL on failure
*/
t_exec_cmd	*build_exec_cmd(t_cmd *p)
{
	t_exec_cmd	*command;

	command = malloc(sizeof(t_exec_cmd));
	if (!command)
		return (NULL);
	command->argv = dup_argv(p->args);
	command->redirs = NULL;
	if (!command->argv)
	{
		free(command);
		return (NULL);
	}
	if (!set_input(command, p))
	{
		free_exec_cmd(command);
		return (NULL);
	}
	if (!set_output(command, p))
	{
		free_exec_cmd(command);
		return (NULL);
	}
	return (command);
}

static int pipeline_fill(t_pipeline *p, t_cmd *parsed)
{
	size_t	i;

	i = 0;
	while (i < p->count)
	{
		p->cmds[i] = build_exec_cmd(parsed);
		if (!p->cmds[i])
		{
			p->count = i;
			free_pipeline(p);
			return (0);
		}
		parsed = parsed->next;
		i++;
	}
	return (1);
}

/*
** pipeline_fill:
** -------------------------------------------------------
** Fills a t_pipeline structure with a list of t_exec_cmd
** built from the linked list of parsed commands.
**
** Behavior:
** - Iterates through the pipeline array
** - Calls build_exec_cmd() for each parsed node
** - On failure:
**       → Frees all already created commands
**       → Sets p->count to the number of valid commands
**       → Frees the pipeline and returns 0
**
** Return:
** - 1 on full success
** - 0 if any command fails to build
*/
t_pipeline	*build_pipeline(t_cmd *parsed)
{
	t_pipeline	*p;
	int			count;

	count = count_parsed_cmds(parsed);
	if (count <= 0)
		return (NULL);
	p = malloc(sizeof(t_pipeline));
	if (!p)
		return (NULL);
	p->cmds = malloc(sizeof(t_exec_cmd *) * count);
	if (!p->cmds)
	{
		free(p);
		return (NULL);
	}
	p->count = count;
	if (!pipeline_fill(p, parsed))
		return (NULL);
	return (p);
}

/*
🇮🇹
🌀​ set_input
Questa funzione controlla se il parser ha trovato un file di input (< file).
-Se non esiste, non fa niente.
-Se esiste, lo aggiunge alla lista delle redirection come R_IN.
-Se add_redir() fallisce, restituisce 0 → errore.

🌀​ set_output
Gestisce la redirezione di output (> o >>).
Se non c’è un file di output → non fa nulla.
Se c’è:
-append_mode == 1 → ridirezione append (>>)
-append_mode == 0 → ridirezione normale (>)
-Se add_redir() fallisce, restituisce 0.

🌀​ build_exec_cmd
Trasforma una singola struttura del parser (t_cmd) in una struttura pronta per l’esecuzione (t_exec_cmd).
Fa:
-Alloca t_exec_cmd
-Duplica gli argomenti (argv)
-Inizializza la lista delle redirezioni
-Aggiunge eventuali input/output redir
-Se qualcosa fallisce → libera tutto e torna NULL.
-È la funzione che “traduce” un comando parsed in un vero comando eseguibile.

🌀​ pipeline_fill
Riempie una t_pipeline con i vari t_exec_cmd generati dai nodi del parser collegati tramite parsed->next.
Se un comando fallisce:
-Setta p->count al numero di comandi validi
-Chiama free_pipeline()
-Restituisce 0
-Serve per costruire correttamente tutti i segmenti della pipeline (A | B | C).

🌀​ build_pipeline
Costruisce l’intera pipeline:
-Conta quanti comandi ci sono (count_parsed_cmds)
-Alloca t_pipeline
-Alloca un array di t_exec_cmd*
-Lo riempie con pipeline_fill()
-Se qualsiasi passo fallisce, restituisce NULL e libera tutto.
-È la funzione che costruisce la pipeline finale che il tuo executor userà per l’esecuzione.
*/
