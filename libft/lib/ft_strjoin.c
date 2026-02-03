/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:38:34 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 11:38:36 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	char	*joined_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	joined_str = malloc(total_len * sizeof(char));
	if (!joined_str)
		return (NULL);
	while (s1[i])
		joined_str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		joined_str[j++] = s2[i++];
	joined_str[j] = '\0';
	return (joined_str);
}

/* 
#include <stdio.h>

int	main(void)
{
	char	*str1;
	char	*str2;
	char	*joined;

    str1 = "Hello ";
    str2 = "World!";
    joined = ft_strjoin(str1, str2);
    printf("Joined string: %s\n", joined);
    free(joined);
    return (0);
} */
