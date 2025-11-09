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

// ======= TOKENIZER =======

typedef enum e_token_type {
    TK_WORD,
    TK_PIPE,        // |
    TK_REDIR_IN,    // <
    TK_REDIR_OUT,   // >
    TK_REDIR_APP,   // >>
    TK_HEREDOC      // <<
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

#endif