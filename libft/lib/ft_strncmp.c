/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:36:23 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 11:36:25 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

/* #include <stdio.h>

int	main(void)
{
	char	str[] = "Hello World!";
    char 	str1[] = "Hello World!";
    
    printf("result: %d\n", ft_strncmp(str, str1, 0));
    printf("standard result: %d\n", strncmp(str, str1, 0));
    return (0);
} */
