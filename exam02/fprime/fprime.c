

#include <stdio.h>
#include <stdlib.h>

int ft_sqrt(int n)
{
	int i = 1;
	while (i * i <= n)
	{
		if (i * i == n)
			return (i);
		i++;
	}
	return (i - 1);
}

int ft_is_prime(int n)
{
	int i;
	int sqrt;

	if (n == 1 || n == 2)
		return (n);
	if (n % 2 == 0)
		return (0);
	i = 2;
	sqrt = ft_sqrt(n) + 1;
	while (i <= sqrt)
	{
		if (n % i == 0)
			return (0);
		++i;
	}
	return (1);
}

void f_prime(int n)
{
	if (n == 1)
		printf("%d", n);

	int i = 2;
	while (n > 1)
	{
		if (ft_is_prime(i) > 0)
		{
			while (n % i == 0)
			{
				n /= i;
				if (n == 1)
					printf("%d", i);
				else 
					printf("%d*", i);
			}
		}
		++i;
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("\n");
		return (0);
	}
	int n = atoi(av[1]);
	f_prime(n);
	printf("\n");
	return (0);
}
// cc -Wall -Wextra -Werror fprime.c -g -o a.out && ./a.out 255255