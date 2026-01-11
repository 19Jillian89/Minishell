/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:20:12 by tvithara          #+#    #+#             */
/*   Updated: 2025/01/22 18:20:14 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putint(int num)
{
	int		len;
	char	*str;

	str = ft_itoa(num);
	len = ft_putstr(str);
	free(str);
	return (len);
}
