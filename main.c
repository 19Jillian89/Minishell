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

int main(int ac, char **av, char **env)
{
    char *line;
	t_shell	shell;

    (void)ac;
    (void)av;

	init_shell(&shell, env); 

    // Set up signal handlers Ctrl-C, Ctrl-D,..
    
    while(42)
    {
        line = readline("minishell> "); 
        if(line == NULL) // Handle Ctr-D (EOF)
        {
            printf("exit\n"); // use libft 
            break;
        }
        if(*line == '\0') // Skip empty input
        {
            free(line);
            continue; 
        }

        add_history(line);  // Add to history

		shell.tokens = tokenize_input(line);	// Tokenization: Convert input string into tokens
		free(line);
		
		if (shell.tokens != NULL)	// If tokens were created successfully, parse them
		{
			// Parsing: Convert tokens into command structures
			shell.commands = parse_tokens(shell.tokens);
			
			// If parsing successful, execute commands
			if (shell.commands != NULL)
			{
				/*
				** EXECUTION PART 
				** The parsed commands are executed
				** shell.commands contains the parsed command structure
				** ready for execution
				*/
				
				// After execution, free the command structures
				free_commands(shell.commands);
				shell.commands = NULL;
			}
			
			free_tokens(shell.tokens); // Free tokens after parsing
			shell.tokens = NULL;
		}
    }
	cleanup_shell(&shell);	// Cleanup before exit
    return(0);
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