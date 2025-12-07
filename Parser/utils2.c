#include "minishell.h"

/*
** Skip over a quoted region without processing the content
*/
int	skip_quoted_region(char *input, int i)
{
	char	quote_type;

	quote_type = input[i];
	i++;
	while (input[i] != '\0' && input[i] != quote_type)
		i++;
	if (input[i] == quote_type)
		i++;
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

/*
** Get environment variable value
*/
/*char	*get_env_value(char *var_name, char **env)
{
	int		i;
	int		len;
	char	*env_var;

	i = 0;
	len = ft_strlen(var_name);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], var_name, len) == 0 && env[i][len] == '=')
		{
			env_var = ft_strdup(env[i] + len + 1);
			return (env_var);
		}
		i++;
	}
	return (NULL);
}
*/

/*
** Extract variable name from string (alphanumeric + underscore)
*/
char	*extract_var_name(char *str, int i)
{
	int	start;
	int	len;

	start = i;
	len = 0;
	while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		i++;
		len++;
	}
	return (ft_substr(str, start, len));
}
