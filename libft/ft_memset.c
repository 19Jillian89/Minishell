/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:30:41 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/22 17:24:46 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *ptr, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;

	i = 0;
	str = (unsigned char *)ptr;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (ptr);
}

/*
** Test

#include <stdio.h>
#include <string.h>

int	main(void)
{
	unsigned int	i;
	char			buffer[];

	i = 0;
	buffer[] = "Hello";
	memset(buffer, 'A', 5);
	printf("memset value: %s\n", buffer);
	ft_memset(buffer, 'A', 5);
	printf("ft_memset value: %s\n", buffer);
	return (0);
}
*/
