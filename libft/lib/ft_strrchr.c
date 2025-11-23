/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:36:09 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 11:36:11 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;

	ptr = NULL;
	while (*s)
	{
		if (*s == (char)c)
			ptr = (char *)s;
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (ptr);
}

/*
**TESTft_isalpha(c) || ft_isdigit(c));n(void)
{
	const char	*str;
	char		c;
	char		*ptr1;
	char		*ptr2;

    str = "Hello World!";
    c = 'h';
    ptr1 = ft_strrchr(str, c);
    ptr2 = strrchr(str, c);
    printf("Last occurrence with of %c in %s is %s\n", c, str, ptr1);
    printf("Last occurrence with of %c in %s is %s\n", c, str, ptr2);
    return (0);
}
*/
