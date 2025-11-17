#include "minishell.h"

/*
** Parse tokens into command structures
** Handles pipes, redirections, and builds command arguments
*/
t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*commands;
	t_cmd	*current_cmd;
	t_token	*current_token;

	commands = NULL;
	current_cmd = NULL;
	current_token = tokens;
	
	while (current_token != NULL && current_token->type != TK_EOF)
	{
		// Start new command for pipes or at beginning
		if (current_cmd == NULL || current_token->type == TK_PIPE)
		{
			if (current_token->type == TK_PIPE)
				current_token = current_token->next;
			current_cmd = create_command_node();
			add_command(&commands, current_cmd);
		}
		// Handle redirections
		else if (is_redirection_token(current_token->type))
			current_token = handle_redirection_parsing(current_token, current_cmd);
		// Handle command arguments
		else if (current_token->type == TK_WORD)
		{
			add_argument(current_cmd, current_token->value);
			current_token = current_token->next;
		}
		else
			current_token = current_token->next;
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
t_token	*handle_redirection_parsing(t_token *token, t_cmd *cmd)
{
	t_token_type	redir_type;
	char			*filename;

	redir_type = token->type;
	token = token->next;
	if (token == NULL || token->type != TK_WORD)
	{
		// Syntax error: redirection without filename
		return (NULL);
	}
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
	// Copy existing arguments
	if (cmd->args != NULL)
	{
		ft_memcpy(new_args, cmd->args, sizeof(char *) * count);
		free(cmd->args);
	}
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	cmd->args = new_args;
}

void	free_commands(t_cmd *commands)
{
	t_cmd	*temp;
	int		i;

	while (commands != NULL)
	{
		temp = commands;
		commands = commands->next;
		if (temp->args != NULL)
		{
			i = 0;
			while (temp->args[i] != NULL)
				free(temp->args[i++]);
			free(temp->args);
		}
		if (temp->input_file != NULL)
			free(temp->input_file);
		if (temp->output_file != NULL)
			free(temp->output_file);
		if (temp->heredoc_delim != NULL)
			free(temp->heredoc_delim);
		free(temp);
	}
}