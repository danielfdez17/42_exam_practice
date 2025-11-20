#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int	ft_abs(int a)
{
	if (a < 0)
		return (-a);
	return (a);
}

bool	is_safe(int *board, int col, int row)
{
	for (int i = 0; i < col; ++i)
	{
		if (board[i] == row) //! ya esta en misma fila
			return false;
		if (ft_abs(board[i] - row) == col - i) //! ya esta en misma diagonal
			return false;
	}
	return true;
}

void	print_board(int *board, int n)
{
	for (int i = 0; i < n; ++i)
	{
		printf("%d", board[i]);
		if (i < n - 1)
			printf(" ");
	}
	printf("\n");
}

void	solve(int *board, int col, int n)
{
	if (n == col)
	{
		print_board(board, n);
		return ;
	}
	for (int row = 0; row < n; ++row) //! recorre todas las filas intentando colocar las reinas
	{
		if (is_safe(board, col, row))
		{
			board[col] = row;
			solve(board, col + 1, n);
		}
	}
}

int main(int ac, char **av)
{
	int n = atoi(av[1]);
	int *board = malloc(sizeof(int) * n);
	if (!board)
		return 1;
	solve(board, 0, n);
	free(board);
	return 0;
}
