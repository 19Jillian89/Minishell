/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:38:05 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 11:38:08 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*ptr;
	size_t	i;

	i = 0;
	ptr = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	while (s[i] != '\0')
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

/*
 #include <stdio.h>

int	main(void)
{
	char	*str;
	char	*dup;

    str = "Hello World!";
    dup = ft_strdup(str);
    printf("Duplicate: %s\n", dup);
    free(dup);
    return (0);
}
 */
