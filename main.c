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
#include "builtin.h"
#include "execution.h"
#include <readline/readline.h>
#include <readline/history.h>

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_shell		shell;

	(void)ac;
	(void)av;
	init_shell(&shell, envp);  
	if (shell.envc.env == NULL)
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
		handle_signal_status(&shell);
		if (process_input_line(line, &shell) == 1)
			break ;
	}
	cleanup_shell(&shell);
	return (shell.exit_status);
}

void	init_shell(t_shell *shell, char **envp)  // modificato
{
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->exit_status = 0;
	// Initialize envc directly inside shell
	if (init_envc(&shell->envc, envp) < 0) 
		shell->envc.env = NULL;
}

void	cleanup_shell(t_shell *shell)
{
	rl_clear_history();  	
	
	if (shell->tokens) 		
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
	free_envc(&shell->envc); 
}

void	handle_signal_status(t_shell *shell)
{
	if (g_signal == SIGINT)
	{
		shell->exit_status = 130;
		shell->envc.exit_code = 130;
		g_signal = 0;
	}
}
