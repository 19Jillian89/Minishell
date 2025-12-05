/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 16:40:31 by ilnassi           #+#    #+#             */
/*   Updated: 2025/12/05 19:04:31 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "minishell.h"
#include "builtin.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

/*
** expand_heredoc_line:
** --------------------------------------
** For now this function simply duplicates the line.
** Normally, a heredoc with expansion enabled should expand variables
** such as $HOME, $USER, etc.
** The 'envc' parameter would be used to perform that expansion.
** Returning a strdup keeps the logic consistent for now.
*/
static char	*expand_heredoc_line(char *line, t_envc *envc)
{
	(void)envc;
	return (ft_strdup(line));
}

/*
** write_line_to_fd:
** --------------------------------------
** Writes the given string to the provided file descriptor,
** followed by a newline.
** Returns -1 on write error, 0 on success.
*/
static int	write_line_to_fd(int fd, char *to_write)
{
	ssize_t	len;
	ssize_t	ret;

	len = ft_strlen(to_write);
	ret = write(fd, to_write, len);
	if (ret == -1)
		return (-1);
	ret = write(fd, "\n", 1);
	if (ret == -1)
		return (-1);
	return (0);
}

/*
** process_line:
** --------------------------------------
** Handles a single line from the heredoc.
**
** If expansion is enabled, the line is passed to expand_heredoc_line(),
** and the original line is freed.
**
** Regardless of expansion, the final string is written to the heredoc file.
**
** Returns 0 on success, -1 on memory/write error.
*/
static int	process_line(char *line, int fd, t_envc *envc, int expand)
{
	char	*to_write;

	if (expand)
	{
		to_write = expand_heredoc_line(line, envc);
		free(line);
		if (!to_write)
			return (-1);
	}
	else
		to_write = line;
	if (write_line_to_fd(fd, to_write) == -1)
	{
		free(to_write);
		return (-1);
	}
	free(to_write);
	return (0);
}

/*
** read_heredoc_loop:
** --------------------------------------
** Main loop that reads user input until the delimiter is encountered.
**
** readline("> ") prints a prompt and returns a new allocated string.
**
** If the user enters the delimiter exactly, we stop reading and free it.
**
** Every other line is processed and written to the heredoc file.
**
** Returns 0 on success, -1 if processing a line fails.
*/
static int	read_heredoc_loop(const char *delim, int fd, t_envc *envc, int exp)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (process_line(line, fd, envc, exp) == -1)
			return (-1);
	}
	return (0);
}

/*
** heredoc_child_process:
** --------------------------------------
** Function executed by the child process when handling a heredoc.
**
** It creates/truncates the target file where the heredoc content is stored.
** Then it enters the reading loop.
**
** On failure, returns 1 (as typical exit status for errors).
** On success, returns 0.
*/
int	heredoc_child_process(const char *delim, const char *filename,
		t_envc *envc, int expand)
{
	int		fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell: heredoc open");
		return (1);
	}
	if (read_heredoc_loop(delim, fd, envc, expand) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/*
- heredoc_child_process() viene eseguito nel processo figlio 
quando nella shell trovi un <<.
-Crea un file temporaneo dove salvare il contenuto del heredoc.
-read_heredoc_loop() usa readline() per leggere riga per riga finché 
l’utente non inserisce il delimitatore.

Se la riga è diversa dal delimitatore:
-viene opzionalmente espansa (variabili shell) tramite process_line();
viene scritta sul file tramite write_line_to_fd().
-La funzione expand_heredoc_line() è un placeholder e al momento non espande variabili, 
ma duplica semplicemente la stringa.
-Se qualcosa va storto (write error, malloc fallito, open fallito), 
il processo figlio ritorna 1.
*/
