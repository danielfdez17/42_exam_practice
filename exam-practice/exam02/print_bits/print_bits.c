/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danfern3 <danfern3@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 07:40:42 by danfern3          #+#    #+#             */
/*   Updated: 2025/10/28 09:01:12 by danfern3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
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

int main(void)
{
	int i = 256;
	while (i--)
	{
		print_bits((unsigned char)i);
		ft_putchar('\n');
	}
	return (0);
}
// clear && cc -Wall -Wextra -Werror print_bits.c -o a.out && ./a.out