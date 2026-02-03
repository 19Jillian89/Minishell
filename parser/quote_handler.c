/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:18:31 by tvithara          #+#    #+#             */
/*   Updated: 2025/12/07 15:18:35 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Process tokens to handle quotes and expansion
** This should be called after tokenization but before command execution
*/
void	process_quotes_and_expansion(t_shell *shell)
{
	t_token			*current_token;
	char			*process_val;
	t_expand_args	expand_args;

	expand_args.shell = shell;
	expand_args.exit_status = shell->exit_status;
	current_token = shell->tokens;
	while (current_token != NULL)
	{
		if (current_token->type == TK_WORD && current_token->value != NULL)
		{
			process_val = rm_quotes_expand(current_token->value, &expand_args);
			if (process_val == NULL)
			{
				ft_putendl_fd("syntax error: unclosed quote", 2);
				shell->exit_status = 2;
				free_tokens(shell->tokens);
				shell->tokens = NULL;
				return ;
			}
			free(current_token->value);
			current_token->value = process_val;
		}
		current_token = current_token->next;
	}
}

/*
** Main function to remove quotes and expand variables
** Handles three contexts: single quotes, double quotes, and unquoted
*/
char	*rm_quotes_expand(char *str, t_expand_args *expand_args)
{
	int	i;
	int	next;

	expand_args->result = ft_strdup("");
	i = 0;
	while (str[i] != '\0')
	{
		next = dispatch_expand(str, i, expand_args);
		if (next == -1)
		{
			free(expand_args->result);
			return (NULL);
		}
		i = next;
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
	if (str[i] == '\0')
		return (-1);
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
	if (str[i] == '\0')
		return (-1);
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
