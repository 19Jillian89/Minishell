/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:03:55 by tvithara          #+#    #+#             */
/*   Updated: 2025/11/09 17:03:58 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

//Standard
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# include <signal.h>

// Readline
#include <readline/readline.h>
#include <readline/history.h>

// Color Codes 
#define RED 	"\033[31m"
#define GREEN 	"\033[32m"
#define YELLOW 	"\033[33m"
#define RESET 	"\033[0m"

// ======= TOKENIZER =======

typedef enum e_token_type {
    TK_WORD,
    TK_PIPE,        // |
    TK_REDIR_IN,    // <
    TK_REDIR_OUT,   // >
    TK_REDIR_APP,   // >>
    TK_HEREDOC,     // <<
	TK_EOF
} t_token_type;

// Token structure
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

// Command structure (for parsing results)
typedef struct s_cmd
{
	char			**args;
	char			*input_file;
	char			*output_file;
	int				append_mode;	// 1 for >>, 0 for >
	int				heredoc_delim;
	struct s_cmd	*next;
}	t_cmd;

// Main shell structure
typedef struct s_shell
{
	t_token		*tokens;
	t_cmd		*commands;
	char		**env;
	int			exit_status;
}	t_shell;

// main
void	init_shell(t_shell *shell, char **env);
void	cleanup_shell(t_shell *shell);

// lexer
t_token	*tokenize_input(char *input);
void	add_token(t_token **token_list, t_token_type type, char *value);
int	handle_redirection(char *input, int i, t_token **tokens);
int	handle_word(char *input, int i, t_token **tokens);
void	free_tokens(t_token *tokens);

#endif
