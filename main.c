/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:03:42 by tvithara          #+#    #+#             */
/*   Updated: 2025/11/09 17:03:46 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"
#include "builtin.h"
#include <readline/readline.h>
#include <readline/history.h>

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_shell		shell;
	t_envc		envc;
	t_pipeline	*pipeline;

	(void)ac;
	(void)av;
	init_shell(&shell, envp);
	if (init_envc(&envc, shell.env) < 0)
		return (1);
	while (1)
	{
		setup_signals_interactive();
		line = readline("minishell> ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (g_signal == SIGINT)
		{
			shell.exit_status = 130;
			envc.exit_code = 130;
			g_signal = 0;
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		shell.tokens = tokenize_input(line);
		free(line);
		if (!shell.tokens)
			continue ;
		process_quotes_and_expansion(&shell);
		shell.commands = parse_tokens(shell.tokens);
		free_tokens(shell.tokens);
		shell.tokens = NULL;
		if (shell.commands)
		{
			pipeline = build_pipeline(shell.commands);
			if (pipeline)
			{
				run_pipeline(pipeline, &envc);
				free_pipeline(pipeline);
			}
			shell.exit_status = envc.exit_code;
			free_commands(shell.commands);
			shell.commands = NULL;
			if (envc.should_exit)
				break ;
		}
	}
	cleanup_shell(&shell);
	free_envc(&envc);
	return (shell.exit_status);
}

void	init_shell(t_shell *shell, char **env)
{
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->env = env;  //  <-- make a copy !!!
	shell->exit_status = 0;
}

void	cleanup_shell(t_shell *shell)
{
	rl_clear_history();  	// Clear readline history
	
	if (shell->tokens) 		// Free any remaining tokens or commands
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
}
