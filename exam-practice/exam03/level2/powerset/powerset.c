#include <stdlib.h>
#include <stdio.h>

void	solve(int *board, int *path, int len, int i, int sum, int target)
{
	if (i == len)
	{
		if (sum == target)
			print_sol(board, path, len);
		return ;
	}
	path[i] = 1;
	solve(board, path, len, i + 1, sum + board[i], target);
	path[i] = 0;
	solve(board, path, len, i + 1, sum, target);
}

int main(int ac, char **av)
{
	if (ac < 2)
		return 1;
	int target = atoi(av[1]);
	int *board = malloc(sizeof(int) * (ac - 2));
	if (!board)
		return 1;
	int *path = calloc(sizeof(int) * (ac - 2));
	if (!path)
		return (free(board), 1);
	int i = -1;
	while (++i < ac - 2)
		board[i] = atoi(av[i + 2]);
	solve(board, path, ac - 2, 0, 0, target);
	free(board);
	free(path);
	return 0;
}
