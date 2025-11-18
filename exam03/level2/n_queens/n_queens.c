#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

bool	is_safe(int *table, int col, int row)
{
	int prev = -1;
	while (++prev < col)
	{
		if (table[prev] == row)
			return (false);
		if (ft_abs(table[prev] - row) == col - prev)
			return (false);
	}
	return (true);
}

void	print_solution(int *table, int n)
{
	int i = -1;
	while (++i < n)
	{
		printf("%d", table[i]);
		if (i < n - 1)
			printf(" ");
	}
	printf("\n");
}

void solve(int *table, int col, int n)
{
	int row = 0;
	if (col == n)
	{
		print_solution(table, n);
		return ;
	}
	while (row < n)
	{
		if (is_safe(table, col, row))
		{
			table[col] = row;
			solve(table, col + 1, n);
		}
		++row;
	}
}

int main(int ac, char **av)
{
	int n;
	int *table;
	if (ac != 2)
		return (1);
	n = atoi(av[1]);
	if (n <= 0)
		return (1);
	table = malloc(sizeof(int) * n);
	if (!table)
		return (1);
	solve(table, 0, n);
	free(table);
	return (0);
}
