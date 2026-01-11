/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:36:45 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 11:36:47 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	unsigned char	*s;

	s = (unsigned char *)str;
	c = (unsigned char)c;
	while (n > 0 && *s != c)
	{
		s++;
		n--;
	}
	if (n)
		return ((void *)s);
	return (NULL);
}
