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

    // Initialize shell structure
	init_shell(&shell, env);   // CAN I USE THIS ???

    // Set up signal handlers Ctrl-C, Ctrl-D,..

    
    while(42)
    {
        line = readline("minishell> "); // Display prompt and read input
        if(!line) // Handle Ctr-D (EOF)
        {
            printf("exit\n");
            break;
        }
        if(*line == '\0') // Skip empty input
        {
            free(line);
            continue;  // IS THIS REALLY NECESSARY?
        }

        // Add to history
        add_history(line);

        // Tokenization: Convert input string into tokens
		shell.tokens = tokenize_input(line);
		free(line);
		
		// If tokens were created successfully, parse them
		if (shell.tokens != NULL)
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
			
			// Free tokens after parsing
			free_tokens(shell.tokens);
			shell.tokens = NULL;
		}
        
        // Cleanup before exit
	    cleanup_shell(&shell);
	    return (0);
    }
    return(0);
}
