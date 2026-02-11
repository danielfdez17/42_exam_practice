#ifndef LIFE_H
#define LIFE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct s_life
{
	int		width;
	int		height;
	int		iterations;
	int		pen_x;
	int		pen_y;
	char	alive;
	char	dead;
	bool	can_write;
	char	**map;
	char	**dup_map;
}	t_life;

static inline bool	is_inside_limits(int x, int y, int height, int width)
{
	return x >= 0 && x < height && y >= 0 && y < width;
}

static inline bool	free_life(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		if (life->map[i])
			free(life->map[i]);
		if (life->dup_map[i])
			free(life->dup_map[i]);
	}
	if (life->map)
		free(life->map);
	if (life->dup_map)
		free(life->dup_map);
	return false;
}

static inline bool	is_alive(int x, int y, t_life *life)
{
	if (is_inside_limits(x, y, life->height, life->width))
		return life->map[x][y] == life->alive;
	return false;
}

static inline bool	is_dead(int x, int y, t_life *life)
{
	if (is_inside_limits(x, y, life->height, life->width))
		return life->map[x][y] == life->dead;
	return false;
}

static inline int	count_neighbours(int x, int y, t_life *life)
{
	int n = 0;

	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i == x && j == y)
				continue;
			if (is_alive(i, j, life))
				n++;
		}
	}
	return n;
}

static inline void conway(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			int n = count_neighbours(i, j, life);
			// printf("n: %d\n", n);
			if (is_alive(i, j, life))
			{
				if (n == 2 || n == 3)
					life->dup_map[i][j] = life->alive;
				else
					life->dup_map[i][j] = life->dead;
			}
			else if (is_dead(i, j, life))
			{
				if (n == 3)
					life->dup_map[i][j] = life->alive;
				else
					life->dup_map[i][j] = life->dead;
			}
		}
	}
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			life->map[i][j] = life->dup_map[i][j];
		}
	}
}

static inline void	play(t_life *life)
{
	while (life->iterations--)
		conway(life);
}

static inline void	print_map(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			putchar(life->map[i][j]);
		}
		putchar('\n');
	}
	// putchar('\n');
}

static inline void	read_input(t_life *life)
{
	char c;
	while (true)
	{
		ssize_t read_bytes = read(STDIN_FILENO, &c, 1);
		if (read_bytes <= 0)
			break ;
		switch (c)
		{
			case 'x':
				life->can_write = !life->can_write;
				break;
			case 'w':
				life->pen_x--;
				break;
			case 'a':
				life->pen_y--;
				break;
			case 's':
				life->pen_x++;
				break;
			case 'd':
				life->pen_y++;
				break;
		}
		if (life->can_write && is_inside_limits(life->pen_x, life->pen_y, life->height, life->width))
			life->map[life->pen_x][life->pen_y] = life->alive;
	}
}

static inline bool	init_life(t_life *life, char **av)
{
	life->width = atoi(av[1]);
	if (life->width <= 0)
		return false;

	life->height = atoi(av[2]);
	if (life->height <= 0)
		return false;

	life->iterations = atoi(av[3]);
	if (life->iterations < 0)
		return false;

	life->pen_x = 0;
	life->pen_y = 0;
	life->alive = 'O';
	life->dead = ' ';
	life->can_write = false;

	life->map = (char **)calloc(sizeof(char **), life->height);
	if (!life->map)
		return false;

	for (int i = 0; i < life->height; i++)
	{
		life->map[i] = (char *)calloc(sizeof(char *), life->width);
		if (!life->map[i])
			return free_life(life);
	}

	life->dup_map = (char **)calloc(sizeof(char **), life->height);
	if (!life->dup_map)
		return free_life(life);

	for (int i = 0; i < life->height; i++)
	{
		life->dup_map[i] = (char *)calloc(sizeof(char *), life->width);
		if (!life->dup_map[i])
			return free_life(life);
	}

	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			life->map[i][j] = life->dead;
			life->dup_map[i][j] = life->dead;
		}
	}

	return true;
}

#endif // LIFE_H