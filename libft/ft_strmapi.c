/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 15:10:45 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/21 15:10:47 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str_new;
	size_t	len;
	size_t	i;

	if (!s || !f)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	str_new = malloc((len + 1) * sizeof(char));
	if (!str_new)
		return (NULL);
	while (i < len)
	{
		str_new[i] = (*f)(i, s[i]);
		i++;
	}
	str_new[i] = '\0';
	return (str_new);
}
