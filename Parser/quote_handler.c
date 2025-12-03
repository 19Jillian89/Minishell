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

char	*remove_quotes_and_expand(char *str, char **env, int exit_status)
{
	char	*result;
	int		i;
	char	quote;

	result = ft_strdup("");
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
		{
			// Single quotes: literal content, no expansion
			i++;
			while (str[i] != '\0' && str[i] != '\'')
			{
				result = ft_strjoin_free(result, ft_substr(str, i, 1));
				i++;
			}
			// CHECK IF CLOSING SINGLE QUOTE EXISTS !!!

			if (str[i] == '\'')
				i++;
		}
		else if (str[i] == '\"')
		{
			// Double quotes: expand variables
			i++;
			while (str[i] != '\0' && str[i] != '\"')
			{
				if (str[i] == '$')
					i = expand_variable_in_dquotes(str, i, &result, env, exit_status);
				else
				{
					result = ft_strjoin_free(result, ft_substr(str, i, 1));
					i++;
				}
			}
			// CHECK IF CLOSING DOUBLE QUOTE EXISTS !!!
			if (str[i] == '\"')
				i++;
		}
		else if (str[i] == '$')
		{
			// Unquoted $ - expand variable
			i = expand_variable(str, i, &result, env, exit_status);
		}
		else
		{
			// Regular character
			result = ft_strjoin_free(result, ft_substr(str, i, 1));
			i++;
		}
	}
	return (result);
}

/*
** Expand variable in double quotes or unquoted context
*/
int	expand_variable_in_dquotes(char *str, int i, char **result, 
							   char **env, int exit_status)
{
	char	*var_name;
	char	*var_value;

	i++; // Skip $
	if (str[i] == '?')
	{
		// Handle $? (exit status)
		var_value = ft_itoa(exit_status);
		*result = ft_strjoin_free(*result, var_value);
		return (i + 1);
	}
	else if (ft_isalpha(str[i]) || str[i] == '_')
	{
		// Handle environment variables
		var_name = extract_var_name(str, i);
		var_value = get_env_value(var_name, env);
		if (var_value != NULL)
			*result = ft_strjoin_free(*result, ft_strdup(var_value));
		free(var_name);
		return (i + ft_strlen(var_name));
	}
	else
	{
		// Not a valid variable, keep as literal 
		*result = ft_strjoin_free(*result, ft_substr(str, i - 1, 2));
		return (i + 1);
	}
}

/*
** Extract variable name from string
*/
char	*extract_var_name(char *str, int i)
{
	int	start;

	start = i;
	while (str[i] != '\0' && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, start, i - start));
}