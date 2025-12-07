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
        if(line == NULL) 
        {
            ft_printf("exit\n");
            break;
        }
        if(*line == '\0') 
        {
            free(line);
            continue; 
        }
        add_history(line);
		shell.tokens = tokenize_input(line);
		free(line);
		if (shell.tokens != NULL)
		{
			process_quotes_and_expansion(&shell);
			shell.commands = parse_tokens(shell.tokens);
			if (shell.commands != NULL)
			{
				//4. EXECUTION PART 
				free_commands(shell.commands);
				shell.commands = NULL;
			}
			free_tokens(shell.tokens); 
			shell.tokens = NULL;
		}
    }
	cleanup_shell(&shell);	
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
	rl_clear_history();  	
	
	if (shell->tokens) 		
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
}