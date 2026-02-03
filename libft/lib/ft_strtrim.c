/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 11:38:55 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 11:38:57 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed;

	if (!s || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s) - 1;
	while (start <= end && ft_strchr(set, s[start]))
		start++;
	while (end >= start && ft_strchr(set, s[end]))
		end--;
	trimmed = ft_substr(s, start, end - start + 1);
	return (trimmed);
}

/*
TEST
int	main(void)
{
	char	*str;
	char	*set;
	char	*trimmed;

    str = "   Hello, World!   ";
    set = "H";
    trimmed = ft_strtrim(str, set);
    printf("Trimmed string: %s\n", trimmed);
    free(trimmed);
    return (0);
}
*/
