/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lcm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danfern3 <danfern3@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 08:16:12 by danfern3          #+#    #+#             */
/*   Updated: 2025/10/28 09:18:12 by danfern3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// * Least Common Multiple
unsigned int	lcm(unsigned int a, unsigned int b)
{
	unsigned int	c = (a > b) ? a : b;
	if (a == 0 || b == 0)
		return (0);
	while (1)
	{
		if ((c % a == 0) && (c % b == 0))
			return (c);
		c++;
	}
}

// * Greatest Common Divisor
unsigned int	gcd(unsigned int a, unsigned int b)
{
	unsigned int	c = (a < b) ? a : b;
	if (a == 0 || b == 0)
		return (0);
	while (c-- > 1)
	{
		if ((a % c == 0) && (b % c == 0))
			return (c);
	}
	return (1);
}