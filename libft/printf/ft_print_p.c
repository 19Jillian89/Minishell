/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:20:30 by tvithara          #+#    #+#             */
/*   Updated: 2025/01/22 18:20:32 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_ptr(unsigned long long ptr);

int	ft_putptr(void *ptr)
{
	unsigned long long	mem_address;
	int					len;

	len = 0;
	if (!ptr)
	{
		len += ft_putstr("(nil)");
		return (len);
	}
	mem_address = (unsigned long long)ptr;
	len += ft_putstr("0x");
	len += ft_printf_ptr(mem_address);
	return (len);
}

int	ft_printf_ptr(unsigned long long ptr)
{
	int	count;

	count = 0;
	if (ptr >= 16)
		count += ft_printf_ptr(ptr / 16);
	if (ptr % 16 <= 9)
		count += ft_putchar((ptr % 16) + '0');
	else
		count += ft_putchar((ptr % 16) - 10 + 'a');
	return (count);
}
