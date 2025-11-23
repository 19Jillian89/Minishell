/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:37:52 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 11:37:54 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*str;

	if (nmemb != 0 && size > SIZE_MAX / nmemb)
		return (NULL);
	str = malloc(size * nmemb);
	if (!str)
		return (NULL);
	ft_bzero(str, size * nmemb);
	return (str);
}
