#include "minishell.h"

/*
** Process tokens to handle quotes and expansion
** This should be called after tokenization but before command execution
*/
void	process_quotes_and_expansion(t_shell *shell)
{
	t_token			*current_token;
	char			*processed_value;
	t_expand_args	expand_args;

	expand_args.exit_status = shell->exit_status;
	current_token = shell->tokens;
	while (current_token != NULL)
	{
		if (current_token->type == TK_WORD && current_token->value != NULL)
		{
			processed_value = remove_quotes_and_expand(current_token->value,
					&expand_args);
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
char	*remove_quotes_and_expand(char *str, t_expand_args *expand_args)
{
	int	i;

	expand_args->result = ft_strdup("");
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			i = handle_single_quotes(str, i, expand_args);
		else if (str[i] == '\"')
			i = handle_double_quotes(str, i, expand_args);
		else if (str[i] == '$')
			i = expand_variable(str, i, expand_args);
		else
		{
			expand_args->result = ft_strjoin_free(expand_args->result,
					ft_substr(str, i, 1));
			i++;
		}
	}
	return (expand_args->result);
}

/*
** Handle single quotes - no expansion, literal content
*/
int	handle_single_quotes(char *str, int i, t_expand_args *expand_args)
{
	i++;
	while (str[i] != '\0' && str[i] != '\'')
	{
		expand_args->result = ft_strjoin_free(expand_args->result,
				ft_substr(str, i, 1));
		i++;
	}
	if (str[i] == '\'')
		i++;
	return (i);
}

/*
** Handle double quotes - expand variables but keep other characters literal
*/
int	handle_double_quotes(char *str, int i, t_expand_args *expand_args)
{
	i++;
	while (str[i] != '\0' && str[i] != '\"')
	{
		if (str[i] == '$')
			i = expand_variable(str, i, expand_args);
		else
		{
			expand_args->result = ft_strjoin_free(expand_args->result,
					ft_substr(str, i, 1));
			i++;
		}
	}
	if (str[i] == '\"')
		i++;
	return (i);
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
