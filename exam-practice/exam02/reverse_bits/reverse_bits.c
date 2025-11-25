/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_bits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danfern3 <danfern3@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 07:54:02 by danfern3          #+#    #+#             */
/*   Updated: 2025/10/28 08:03:11 by danfern3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	int i = 0;
	while (str && str[i])
		ft_putchar(str[i++]);
	ft_putchar('\n');
}

void	print_bits(unsigned char octet)
{
	int	i = 8 * sizeof(unsigned char);
	unsigned char bit = 0;
	while (i--)
	{
		bit = (octet >> i & 1) + 48;
		ft_putchar(bit);
	}
}

unsigned char	reverse_bits(unsigned char octet)
{
	unsigned char	result = 0;
	int	i = 8 * sizeof(unsigned char);
	while (i--)
	{
		result = (result << 1) | (octet & 1);
		octet = octet >> 1;
	}
	return (result);
}

int main(void)
{
	int i = 10;
	while (i--)
	{
		ft_putstr("before reverse: ");
		print_bits((unsigned char)i);
		ft_putchar('\n');
		ft_putstr("after reverse: ");
		print_bits(reverse_bits((unsigned char)i));
		ft_putchar('\n');
	}
	return (0);
}