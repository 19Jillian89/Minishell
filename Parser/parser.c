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