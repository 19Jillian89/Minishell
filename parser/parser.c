/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:18:10 by tvithara          #+#    #+#             */
/*   Updated: 2026/02/03 17:25:23 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

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
	cmd->redirs = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delim = NULL;
	cmd->next = NULL;
	return (cmd);
}

/* add_redir() dovrebbe fare la strdup interna, ma non lo so nel tuo progetto.
	   Per sicurezza: se add_redir NON duplica, allora NON free(target).*/
// DA RIVEDERE 
t_token	*handle_redir_parsing(t_token *token, t_cmd *cmd)
{
	t_redir_type	r_type;
	char			*target;

	token = token->next;
	if (token == NULL || token->type != TK_WORD)
		return (NULL);
	target = ft_strdup(token->value);
	if (!target)
		return (NULL);
	if (token->type == TK_REDIR_IN)
		r_type = R_IN;
	else if (token->type == TK_REDIR_OUT)
		r_type = R_OUT;
	else if (token->type == TK_REDIR_APP)
		r_type = R_APP;
	else
		r_type = R_HEREDOC;
	if (!add_redir(&cmd->redirs, r_type, target))
	{
		free(target);
		return (NULL);
	}
	free(target);
	return (token->next);
}

/*
** Add an argument to a command's argument array
*/
void	add_argument(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_args;

	if (!arg || arg[0] == '\0')
		return ;
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

t_token	*handle_new_cmd(t_token *tk, t_cmd **cmds, t_cmd **current_cmd)
{
	if (tk->type == TK_PIPE)
		tk = tk->next;
	*current_cmd = create_command_node();
	add_command(cmds, *current_cmd);
	return (tk);
}
