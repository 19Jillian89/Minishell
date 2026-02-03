/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:19:23 by tvithara          #+#    #+#             */
/*   Updated: 2025/01/22 18:19:31 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthex(unsigned int num, char c)
{
	int	count;

	count = 0;
	if (num == 0)
		return (ft_putchar('0'));
	if (num >= 16)
		count += ft_puthex(num / 16, c);
	if (num % 16 <= 9)
		count += ft_putchar((num % 16) + '0');
	else
	{
		if (c == 'x')
			count += ft_putchar((num % 16) - 10 + 'a');
		else
			count += ft_putchar((num % 16) - 10 + 'A');
	}
	return (count);
}
