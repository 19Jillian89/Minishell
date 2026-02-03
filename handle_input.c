#include "minishell.h"
#include "execution.h"
#include <readline/history.h>

static int	parse_and_execute(t_shell *shell)
{
	t_pipeline	*pipeline;

	shell->commands = parse_tokens(shell->tokens);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	if (!shell->commands)
		return (0);
	pipeline = build_pipeline(shell->commands);
	if (pipeline)
	{
		run_pipeline(pipeline, shell);
		free_pipeline(pipeline);
	}
	shell->exit_status = shell->envc.exit_code;
	free_commands(shell->commands);
	shell->commands = NULL;
	if (shell->envc.should_exit)
		return (1);
	return (0);
}

static int	tokenize_and_process(char *line, t_shell *shell)
{
	shell->tokens = tokenize_input(line);
	free(line);
	if (!shell->tokens)
		return (0);
	process_quotes_and_expansion(shell);
	if (!shell->tokens)
		return (0);
	return (parse_and_execute(shell));
}

int	process_input_line(char *line, t_shell *shell)
{
	if (*line == '\0')
	{
		free(line);
		return (0);
	}
	add_history(line);
	return (tokenize_and_process(line, shell));
}
