/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 12:46:13 by tvithara          #+#    #+#             */
/*   Updated: 2024/12/22 17:23:36 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numlen(int n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		len++;
		n = -n;
	}
	if (n == 0)
		return (1);
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static void	check_max(int *nbr, char *s)
{
	if (*nbr == -2147483648)
	{
		s[0] = '-';
		s[1] = '2';
		*nbr = 147483648;
	}
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;

	len = ft_numlen(n);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	len--;
	check_max(&n, str);
	if (n < 0)
	{
		str[0] = '-';
		n = -n;
	}
	if (n == 0)
		str[0] = '0';
	while (n > 0)
	{
		str[len] = (n % 10) + '0';
		n /= 10;
		len--;
	}
	return (str);
}

/* #include <stdio.h>

int	main(void)
{
	int		num;
	int		num1;
	char	*result;

	num = 0;
	num1 = -2147483648;
	result = ft_itoa(num);
	printf("result: %s\n", result);
	free(result);
	return (0);
}
 */