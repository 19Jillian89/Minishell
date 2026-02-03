/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvithara <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 19:44:31 by tvithara          #+#    #+#             */
/*   Updated: 2025/01/16 19:44:34 by tvithara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

int	ft_printf(const char *format, ...);

int	ft_putchar(char c);
int	ft_putstr(char *str);

int	ft_putptr(void *ptr);
int	ft_putint(int num);
int	ft_puthex(unsigned int num, char c);

int	ft_put_u(unsigned int num);
#endif
