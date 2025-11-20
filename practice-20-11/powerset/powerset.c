#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void	print_path(int *board, int *path, int len)
{
	bool first = true;
	for (int i = 0; i < len; ++i)
	{
		if (path[i])
		{
			if (!first)
				printf(" ");
			printf("%d", board[i]);
			first = false;
		}
	}
	printf("\n");
}

void solve(int *board, int *path, int len, int i, int sum, int target)
{
	if (i == len)
	{
		if (sum == target)
			print_path(board, path, len);
		return;
	}
	path[i] = true;
	solve(board, path, len, i + 1, sum + board[i], target);
	path[i] = false;
	solve(board, path, len, i + 1, sum, target);

}
int main(int ac, char **av)
{
	if (ac == 1)
		return 1;
	int target = atoi(av[1]);
	int *board = malloc(sizeof(int) * (ac - 2));
	if (!board)
		return 1;
	int *path = calloc(sizeof(int), (ac - 2));
	if (!path)
		return (free(board), 1);
	for (int i = 0; i < ac - 2; ++i)
		board[i] = atoi(av[i + 2]);
	solve(board, path, ac - 2, 0, 0, target);
	free(board);
	free(path);
	return 0;
}