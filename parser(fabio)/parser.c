/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:18:10 by tvithara          #+#    #+#             */
/*   Updated: 2025/12/07 15:18:15 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_new_cmd(t_token *tk, t_cmd **cmds, t_cmd **current_cmd);

/*
** Parse tokens into command structures
** Handles pipes, redirections, and builds command arguments
*/
t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*commands;
	t_cmd	*current_cmd;
	t_token	*current_tk;

	commands = NULL;
	current_cmd = NULL;
	current_tk = tokens;
	while (current_tk != NULL && current_tk->type != TK_EOF)
	{
		if (current_cmd == NULL || current_tk->type == TK_PIPE)
			current_tk = handle_new_cmd(current_tk, &commands, &current_cmd);
		else if (is_redirection_token(current_tk->type))
			current_tk = handle_redir_parsing(current_tk, current_cmd);
		else if (current_tk->type == TK_WORD)
		{
			add_argument(current_cmd, current_tk->value);
			current_tk = current_tk->next;
		}
		else
			current_tk = current_tk->next;
	}
	return (commands);
}


/*
** Create a new command node with initialized values
*/
t_cmd	*create_command_node(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	return (cmd);
}

/*
** Handle parsing of redirection tokens and their following file names
*/
t_token	*handle_redir_parsing(t_token *token, t_cmd *cmd)
{
	t_token_type	redir_type;
	char			*filename;

	redir_type = token->type;
	token = token->next;
	if (token == NULL || token->type != TK_WORD)
		return (NULL);
	filename = ft_strdup(token->value);
	if (redir_type == TK_REDIR_IN)
		cmd->input_file = filename;
	else if (redir_type == TK_REDIR_OUT)
	{
		cmd->output_file = filename;
		cmd->append_mode = 0;
	}
	else if (redir_type == TK_REDIR_APP)
	{
		cmd->output_file = filename;
		cmd->append_mode = 1;
	}
	else if (redir_type == TK_HEREDOC)
		cmd->heredoc_delim = filename;
	return (token->next);
}

/*
** Add an argument to a command's argument array
*/
void	add_argument(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_args;

	count = 0;
	if (cmd->args != NULL)
	{
		while (cmd->args[count] != NULL)
			count++;
	}
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	if (cmd->args != NULL)
	{
		ft_memcpy(new_args, cmd->args, sizeof(char *) * count);
		free(cmd->args);
	}
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	cmd->args = new_args;
}

static t_token	*handle_new_cmd(t_token *tk, t_cmd **cmds, t_cmd **current_cmd)
{
	if (tk->type == TK_PIPE)
		tk = tk->next;
	*current_cmd = create_command_node();
	add_command(cmds, *current_cmd);
	return (tk);
}

/*

Casi limite / possibili errori nel codice attuale

    1) Redirezione senza filename
    	handle_redirection_parsing se trova token == NULL o token->type != TK_WORD ritorna NULL. 
		parse_tokens assegna current_token = NULL e il while termina senza segnalare errore esplicito. 
		Questo può far perdere informazione di errore e lasciare strutture parziali in memoria.
    
	2) Pipe all'inizio o alla fine / pipe consecutiva
        Se la linea comincia con '|' o finisce con '|' o ci sono 'cmd1 || cmd2' (due pipe di seguito), 
		il codice creerà nodi comando vuoti (current_cmd creato ma senza argomenti). Non c'è controllo per sintassi errata.

    3) Allocazioni non controllate
        create_command_node può ritornare NULL ma parse_tokens non verifica il valore ritornato prima di usarlo.
        add_argument fa malloc per il nuovo array e per la stringa, ma non gestisce bene fallimenti (se malloc fallisce non libera).

    4) Ownership della memoria
        handle_redirection_parsing duplica il filename con ft_strdup; add_argument duplica arg; free_commands libera queste stringhe: la proprietà è coerente,
		 ma bisogna assicurarsi che ogni strdup abbia il suo free.

    5) is_redirection_token non è mostrata: bisogna garantire che identifichi correttamente TK_REDIR_IN, TK_REDIR_OUT, TK_REDIR_APP, TK_HEREDOC.

    6) Mancata gestione di quote/espansione/escape
        Il parser presume che il lexer abbia già gestito quote e abbia prodotto token TK_WORD corretti. Se il lexer non l'ha fatto, bisogna occuparsene.


*/
