/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:04:10 by tvithara          #+#    #+#             */
/*   Updated: 2025/11/09 17:04:12 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Tokenize the input string into a linked list of tokens
** Handles words, pipes, redirections, and quotes
*/

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		// Skip whitespace
		if (is_whitespace(input[i]))
		{
			i++;
			//continue ;
		}
		
		// Handle pipes
		if (input[i] == '|')
		{
			add_token(&tokens, TK_PIPE, "|");
			i++;
		}
		// Handle redirections
		else if (input[i] == '<' || input[i] == '>')
			i = handle_redirection(input, i, &tokens);
		// Handle words (including quoted strings)
		else
			i = handle_word(input, i, &tokens);
	}
	add_token(&tokens, TK_EOF, NULL);
	return (tokens);
}

/*
** Add a new token to the token list
*/
void	add_token(t_token **token_list, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->type = type;
	if (value)
		new_token->value = ft_strdup(value);
	else
		new_token->value = NULL;
	new_token->next = NULL;
	if (*token_list == NULL)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_token;
	}
}

/*
** Handle redirection tokens: <, >, <<, >>
*/
int	handle_redirection(char *input, int i, t_token **tokens)
{
	if (input[i] == '<')
	{
		if (input[i + 1] == '<')
		{
			add_token(tokens, TK_HEREDOC, "<<");
			return (i + 2);
		}
		add_token(tokens, TK_REDIR_IN, "<");
		return (i + 1);
	}
	else if (input[i] == '>')
	{
		if (input[i + 1] == '>')
		{
			add_token(tokens, TK_REDIR_APP, ">>");
			return (i + 2);
		}
		add_token(tokens, TK_REDIR_OUT, ">");
		return (i + 1);
	}
	return (i);
}

/*
** Handle word tokens, including quoted strings
** This function extracts words while handling single and double quotes
*/
int	handle_word(char *input, int i, t_token **tokens)
{
	int		start;
	char	*word;
	
	start = i;
	while (input[i] != '\0' && !is_whitespace(input[i]) && 
		   !is_metacharacter(input[i]))
	{
		// Handle quoted strings
		if (input[i] == '\'' || input[i] == '\"')
			i = handle_quoted_string(input, i, &word);
		else
			i++;
	}
	word = ft_substr(input, start, i - start);
	if (word != NULL)
	{
		add_token(tokens, TK_WORD, word);
		free(word);
	}
	return (i);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens != NULL)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->value != NULL)
			free(temp->value);
		free(temp);
	}
}