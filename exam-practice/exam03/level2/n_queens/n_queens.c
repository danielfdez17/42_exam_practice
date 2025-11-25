#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void	print_sol(int *board, int n)
{
	bool first = true;
	for (int i = 0; i < n; ++i)
	{
		if (!first)
			printf(" ");
		printf("%d", board[i]);
		first = false;
	}
	printf("\n");
}

int ft_abs(int a)
{
	if (a < 0)
		return -a;
	return a;
}

bool	ft_is_safe(int *board, int col, int row)
{
	for (int i = 0; i < col; ++i)
	{
		if (board[i] == row)
			return false;
		if (ft_abs(board[i] - row) == col - i)
			return false;
	}
	return true;
}

void	solve(int *board, int col, int n)
{
	if (col == n)
	{
		print_sol(board, n);
		return ;
	}
	for (int row = 0; row < n; ++row)
	{
		if (ft_is_safe(board, col, row))
		{
			board[col] = row;
			solve(board, col + 1, n);
		}
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	int n = atoi(av[1]);
	if (n <= 0)
		return 1;
	int *board = malloc(sizeof(int) * n);
	if (!board)
		return 1;
	solve(board, 0, n);
	free(board);
	return 0;
}
