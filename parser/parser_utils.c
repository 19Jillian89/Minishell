/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:00:25 by tvithara          #+#    #+#             */
/*   Updated: 2026/02/05 16:00:29 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Process a single token based on its type
*/
t_token	*process_token(t_token *tk, t_cmd **cmds, t_cmd **current_cmd)
{
	if (*current_cmd == NULL || tk->type == TK_PIPE)
	{
		if (tk->type == TK_PIPE && *current_cmd == NULL)
		{
			ft_putendl_fd("syntax error near unexpected token `|'", 2);
			return (NULL);
		}
		return (handle_new_cmd(tk, cmds, current_cmd));
	}
	else if (is_redirection_token(tk->type))
		return (handle_redir_parsing(tk, *current_cmd));
	else if (tk->type == TK_WORD)
	{
		add_argument(*current_cmd, tk->value);
		return (tk->next);
	}
	return (tk->next);
}

char	*get_redir_target(t_token *token)
{
	if (!token || token->type != TK_WORD)
	{
		ft_putendl_fd("syntax error near unexpected token `newline'", 2);
		return (NULL);
	}
	return (ft_strdup(token->value));
}
