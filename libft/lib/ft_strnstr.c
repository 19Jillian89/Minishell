/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:37:28 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/22 17:31:00 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	size_t	needle_len;

	if (n == 0)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	while (n >= needle_len && *haystack)
	{
		if (ft_strncmp(haystack, needle, needle_len) == 0)
			return ((char *)haystack);
		haystack++;
		n--;
	}
	return (NULL);
}

/* #include <stdio.h>
int	main(void)
{
	char	str[] = "Hello World!";
	char	*needle;
	char	*result;

    
    needle = "World";
    result = ft_strnstr(str, needle, 11);
    if (result)
    {
        printf("'%s' found in '%s'\n", needle, str);
        printf("result: %s\n", result);
    }
    else
        printf("'%s' not found in '%s'\n", needle, str);
    return (0);
} */
