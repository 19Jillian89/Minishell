/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:35:08 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 11:35:11 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;
	size_t	i;

	i = 0;
	d_len = ft_strlen(dest);
	s_len = ft_strlen(src);
	if (size <= d_len)
		return (size + s_len);
	while (src[i] != '\0' && (d_len + i) < (size - 1))
	{
		dest[d_len + i] = src[i];
		i++;
	}
	dest[d_len + i] = '\0';
	return (d_len + s_len);
}

/*
**TEST

#include <string.h>

int	main(void)
{
	char	dest1[100];
	char	dest2[100];
	char	src[];

    dest1[100] = "Hello";
    dest2[100] = "Hello";
    src[] = " World";
    printf("count of ft_strlcat: %lu\n", ft_strlcat(dest1, src, 12));
    printf("count of strlcat: %lu\n", strlcat(dest2, src, 12));
    printf("New dest after ft_strlcat: %s\n", dest1);
    printf("New dest after strlcat: %s\n", dest2);
    return (0);
}
*/
