#include "minishell.h"

/*
** Skip over a quoted region without processing the content
*/
int	skip_quoted_region(char *input, int i)
{
	char	quote_type;

	quote_type = input[i];
	i++; // Skip opening quote
	
	// Find closing quote
	while (input[i] != '\0' && input[i] != quote_type)
		i++;
	
	if (input[i] == quote_type)
		i++; // Skip closing quote
	
	return (i);
}

/*
** Free the linked list of command structures
*/
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