/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:47:02 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:45:59 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ===========================
**  INCLUDES
** =========================== */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <stddef.h>

/* ATTENZIONE: aggiusta il path di libft in base al tuo progetto */
# include "../libft/libft.h"

/* ===========================
**  PARSER STRUCTS (FABIO)
** =========================== */

typedef enum e_token_type
{
    TK_WORD,
    TK_PIPE,        // |
    TK_REDIR_IN,    // <
    TK_REDIR_OUT,   // >
    TK_REDIR_APP,   // >>
    TK_HEREDOC,     // <<
    TK_EOF
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct s_cmd
{
    char            **args;         // argv del comando
    char            *input_file;    // "< file"
    char            *output_file;   // "> file" o ">> file"
    int             append_mode;    // 1 per >>, 0 per >
    char            *heredoc_delim;  
    struct s_cmd    *next;          // comando successivo nella pipeline
}   t_cmd;

typedef struct s_shell
{
    t_token *tokens;
    t_cmd   *commands;
    char    **env;
    int     exit_status;
}   t_shell;

/* ===========================
**  ENV WRAPPER (TUO)
** =========================== */

typedef struct s_envc
{
    char    **env;         // ambiente modificabile
    int     exit_code;     // ultimo exit status
    int     should_exit;   // flag per sapere se uscire dalla shell
    int		heredoc_id;
}   t_envc;

int     init_envc(t_envc *envc, char **envp);
void    free_envc(t_envc *envc);

/* ===========================
**  PARSER API (FABIO)
** =========================== */

/* main / shell init */
void    init_shell(t_shell *shell, char **env);
void    cleanup_shell(t_shell *shell);

/* lexer */
t_token *tokenize_input(char *input);
void    add_token(t_token **token_list, t_token_type type, char *value);
int     handle_redirection(char *input, int i, t_token **tokens);
int     handle_word(char *input, int i, t_token **tokens);
int	skip_quoted_string(const char *input, int i, char **word);
int     skip_quoted_region(char *input, int i);
void    free_tokens(t_token *tokens);

/* parser */
t_cmd   *parse_tokens(t_token *tokens);
t_cmd   *create_command_node(void);
t_token *handle_redirection_parsing(t_token *token, t_cmd *cmd);
void    add_argument(t_cmd *cmd, char *arg);
void    free_commands(t_cmd *commands);
void	add_command(t_cmd **list, t_cmd *new_cmd);

/* quote handler */
void    process_quotes_and_expansion(t_shell *shell);
char    *remove_quotes_and_expand(char *str, char **env, int exit_status);
int     expand_variable_in_dquotes(char *str, int i, char **result,
                                   char **env, int exit_status);
char    *extract_var_name(char *str, int i);
int	expand_variable(char *str, int i, char **result, char **env, int exit_status);

/* utils (parser) */
int     is_whitespace(char c);
int     is_metachar(char c);
int     is_redirection_token(t_token_type type);
char    *ft_strjoin_free(char *s1, char *s2);
void    print_error(char *msg);

extern volatile sig_atomic_t	g_signal;

void    setup_signals_interactive(void);
void    setup_signals_child(void);
void    setup_signals_parent_exec(void);

#endif
