#include "minishell.h"

int main(int ac, char **av, char **env)
{
    char *line;

    (void)ac;
    (void)av;

    // Initialize shell structure

    // Set up signal handlers
    
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
        
    }
    return(0);
}