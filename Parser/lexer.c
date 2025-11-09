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
			continue ;
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
