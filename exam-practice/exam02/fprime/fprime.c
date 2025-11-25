/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fprime.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danfern3 <danfern3@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 08:19:39 by danfern3          #+#    #+#             */
/*   Updated: 2025/10/28 08:24:54 by danfern3         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

void	fprime(int n)
{
	int i = 2;
	if (n == 1)
		printf("1");
	while (i <= n)
	{
		while (n % i == 0)
		{
			printf("%d", i);
			if (n != i)
				printf("*");
			n /= i;
		}
		++i;
	}
	printf("\n");
}

int main(int ac, char **av)
{
	int n, i = 1;
	if (ac >= 2)
	{
		while (i < ac)
		{
			n = atoi(av[i]);
			printf("%d = ", n);
			fprime(n);
			++i;
		}
	}
	return (0);
}