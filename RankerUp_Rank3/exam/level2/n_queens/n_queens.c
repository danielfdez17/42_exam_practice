#include <stdlib.h>
#include <stdio.h>

int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

int	is_safe(int *board, int col, int row)
{
	int	prev = 0;

	while (prev < col)
	{
		if (board[prev] == row) //! Ya hay otra reina en la misma fila
			return (0);
		if (ft_abs(board[prev] - row) == col - prev) //! Ya hay otra reina en la misma diagonal
			return (0);
		prev++;
	}
	return (1);
}

void	print_solution(int *board, int n)
{
	int	i = 0;

	while (i < n)
	{
		fprintf(stdout, "%d", board[i]);
		if (i < n - 1)
			fprintf(stdout, " ");
		i++;
	}
	fprintf(stdout, "\n");
}

void	solve(int *board, int col, int n)
{
	int	row = 0;

	if (col == n) //! todas las reinas ya colocadas
	{
		print_solution(board, n);
		return ;
	}
	while (row < n) //! Intentar colocar una reina en la en la columna actual
	{
		if (is_safe(board, col, row)) //! Si es seguro colocarla, se coloca y se pasa a la siguiente columna
		{
			board[col] = row;
			solve(board, col + 1, n);
		}
		row++;
	}
}

int	main(int ac, char **av)
{
	int	n;
	int	*board;

	if (ac != 2)
		return (1);
	n = atoi(av[1]);
	if (n <= 0)
		return (1);
	board = malloc(sizeof(int) * n);
	if (!board)
		return (1);
	solve(board, 0, n);
	free(board);
	return (0);
}