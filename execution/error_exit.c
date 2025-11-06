/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:42:14 by ilnassi           #+#    #+#             */
/*   Updated: 2025/11/06 18:42:23 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Gestisce un errore critico stampando il messaggio
 * e terminando il programma con EXIT_FAILURE. */
void	error_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
