#include "minishell.h"

/*
** Process tokens to handle quotes and expansion
** This should be called after tokenization but before command execution
*/
void	process_quotes_and_expansion(t_shell *shell)
{
	t_token	*current_token;
	char	*processed_value;

	current_token = shell->tokens;
	while (current_token != NULL)
	{
		if (current_token->type == TK_WORD && current_token->value != NULL)
		{
			processed_value = remove_quotes_and_expand(current_token->value, 
									shell->env, shell->exit_status);
			if (processed_value != NULL)
			{
				free(current_token->value);
				current_token->value = processed_value;
			}
		}
		current_token = current_token->next;
	}
}

/*
** Main function to remove quotes and expand variables
** Handles three contexts: single quotes, double quotes, and unquoted
*/
char	*remove_quotes_and_expand(char *str, char **env, int exit_status)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			i = handle_single_quotes(str, i, &result);
		else if (str[i] == '\"')
			i = handle_double_quotes(str, i, &result, env, exit_status);
		else if (str[i] == '$')
			i = expand_variable(str, i, &result, env, exit_status);
		else
		{
			result = ft_strjoin_free(result, ft_substr(str, i, 1));
			i++;
		}
	}
	return (result);
}

/*
** Handle single quotes - no expansion, literal content
*/
int	handle_single_quotes(char *str, int i, char **result)
{
	i++; // Skip opening quote
	
	// Copy everything until closing quote
	while (str[i] != '\0' && str[i] != '\'')
	{
		*result = ft_strjoin_free(*result, ft_substr(str, i, 1));
		i++;
	}
	
	// Skip closing quote if found
	if (str[i] == '\'')
		i++;
	
	return (i);
}

/*
** Handle double quotes - expand variables but keep other characters literal
*/
int	handle_double_quotes(char *str, int i, char **result, 
						char **env, int exit_status)
{
	i++; // Skip opening quote
	
	while (str[i] != '\0' && str[i] != '\"')
	{
		if (str[i] == '$')
			i = expand_variable_in_dquotes(str, i, result, env, exit_status);
		else
		{
			*result = ft_strjoin_free(*result, ft_substr(str, i, 1));
			i++;
		}
	}
	
	// Skip closing quote if found
	if (str[i] == '\"')
		i++;
	
	return (i);
}

/*
** Expand variable in double quotes context
** Returns new position in string
*/
int	expand_variable_in_dquotes(char *str, int i, char **result, 
							   char **env, int exit_status)
{
	char	*var_name;
	char	*var_value;

	i++; // Skip $
	
	// Handle $? (exit status)
	if (str[i] == '?')
	{
		var_value = ft_itoa(exit_status);
		*result = ft_strjoin_free(*result, var_value);
		return (i + 1);
	}
	// Handle environment variables
	else if (ft_isalpha(str[i]) || str[i] == '_')
	{
		var_name = extract_var_name(str, i);
		var_value = get_env_value(var_name, env);
		if (var_value != NULL)
		{
			*result = ft_strjoin_free(*result, ft_strdup(var_value));
			free(var_value);
		}
		free(var_name);
		return (i + ft_strlen(var_name));
	}
	else
	{
		// Not a valid variable, keep $ as literal
		*result = ft_strjoin_free(*result, ft_substr(str, i - 1, 1));
		return (i);
	}
}

/*
** Expand variable in unquoted context
** Returns new position in string
*/
int	expand_variable(char *str, int i, char **result, 
					char **env, int exit_status)
{
	char	*var_name;
	char	*var_value;

	i++; // Skip $
	
	// Handle $? (exit status)
	if (str[i] == '?')
	{
		var_value = ft_itoa(exit_status);
		*result = ft_strjoin_free(*result, var_value);
		return (i + 1);
	}
	// Handle environment variables
	else if (ft_isalpha(str[i]) || str[i] == '_')
	{
		var_name = extract_var_name(str, i);
		var_value = get_env_value(var_name, env);
		if (var_value != NULL)
		{
			*result = ft_strjoin_free(*result, ft_strdup(var_value));
			free(var_value);
		}
		free(var_name);
		return (i + ft_strlen(var_name));
	}
	else
	{
		// Not a valid variable, keep $ as literal
		*result = ft_strjoin_free(*result, ft_substr(str, i - 1, 1));
		return (i);
	}
}

/*
** Extract variable name from string (alphanumeric + underscore)
*/
char	*extract_var_name(char *str, int i)
{
	int		start;
	int		len;

	start = i;
	len = 0;
	
	while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		i++;
		len++;
	}
	
	return (ft_substr(str, start, len));
}

/*
** Get environment variable value
*/
char	*get_env_value(char *var_name, char **env)
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

/*
** Special function to handle heredoc delimiter (no expansion)
*/
char	*handle_heredoc_delimiter(char *str)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			// For heredoc, quotes are part of the delimiter
			result = ft_strjoin_free(result, ft_substr(str, i, 1));
			i++;
		}
		else
		{
			result = ft_strjoin_free(result, ft_substr(str, i, 1));
			i++;
		}
	}
	return (result);
}