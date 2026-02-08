#ifndef LIFE_H
#define LIFE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct s_life
{
	int		witdh;
	int		height;
	int		iterations;
	bool	can_write;
	char	alive;
	char	dead;
	int		pen_x;
	int		pen_y;
	char	**map;
	char	**dup_map;
}	t_life;

static inline bool	free_map(t_life *life)
{
	int i = -1;
	while (++i < life->height)
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

static inline bool	init_life(t_life *life, char **av)
{
	life->witdh = atoi(av[1]);
	if (life->witdh <= 0)
		return false;
	life->height = atoi(av[2]);
	if (life->height <= 0)
		return false;
	life->iterations = atoi(av[3]);
	if (life->iterations < 0)
		return false;
	life->can_write = false;

	life->map = (char **)calloc(sizeof(char *), life->height);
	if (!life->map)
		return false;
	
	for (int i = 0; i < life->height; i++)
	{
		life->map[i] = (char *)calloc(sizeof(char), life->witdh);
		if (!life->map[i])
			return free_map(life);
	}

	life->dup_map = (char **)calloc(sizeof(char *), life->height);
	if (!life->dup_map)
		return free_map(life);
	
	for (int i = 0; i < life->height; i++)
	{
		life->dup_map[i] = (char *)calloc(sizeof(char), life->witdh);
		if (!life->dup_map[i])
			return free_map(life);
	}

	life->alive = 'O';
	life->dead = ' ';
	life->pen_x = 0;
	life->pen_y = 0;

	for (int i = 0; i < life->height; i++)
		for (int j = 0; j < life->witdh; j++)
			life->map[i][j] = life->dead;
	
	return true;
}

static inline bool	is_inside_limits(int x, int y, t_life *life)
{
	return x >= 0 && x < life->height && y >= 0 && y < life->witdh;
}

static inline bool	is_alive(int x, int y, t_life *life)
{
	if (!is_inside_limits(x, y, life))
		return false;
	return life->map[x][y] == life->alive;
}

static inline bool	is_dead(int x, int y, t_life *life)
{
	if (!is_inside_limits(x, y, life))
		return false;
	return life->map[x][y] == life->dead;
}

static inline int	count_neighbours(int x, int y, t_life *life)
{
	int n = 0;
	// if (is_inside_limits(x - 1, y - 1, life) && is_alive(x - 1, y - 1, life))
	// 	n++;
	// if (is_inside_limits(x - 1, y, life) && is_alive(x - 1, y, life))
	// 	n++;
	// if (is_inside_limits(x - 1, y + 1, life) && is_alive(x - 1, y + 1, life))
	// 	n++;
	// if (is_inside_limits(x, y - 1, life) && is_alive(x, y - 1, life))
	// 	n++;
	// if (is_inside_limits(x, y + 1, life) && is_alive(x, y + 1, life))
	// 	n++;
	// if (is_inside_limits(x + 1, y - 1, life) && is_alive(x + 1, y - 1, life))
	// 	n++;
	// if (is_inside_limits(x + 1, y, life) && is_alive(x + 1, y, life))
	// 	n++;
	// if (is_inside_limits(x + 1, y + 1, life) && is_alive(x + 1, y + 1, life))
	// 	n++;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (i == x && j == y)
				continue;
			if (is_inside_limits(i, j, life) && is_alive(i, j, life))
				n++;
		}
	}
	return n;
}

static inline void	conway(t_life *life)
{
	// printf("Unimplemented method\n");
	int	neighbours = 0;
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->witdh; j++)
		{
			neighbours = count_neighbours(i, j, life);
			// printf("(%d,%d): %d\n", i, j, neighbours);
			if (is_alive(i, j, life))
			{
				if (neighbours < 2 || neighbours > 3)
					life->dup_map[i][j] = life->dead;
				else
					life->dup_map[i][j] = life->map[i][j];
			}
			else if (is_dead(i, j, life))
			{
				if (neighbours == 3)
					life->dup_map[i][j] = life->alive;
				else
					life->dup_map[i][j] = life->map[i][j];
			}
		}
	}
	for (int i = 0; i < life->height; i++)
		for (int j = 0; j < life->witdh; j++)
			life->map[i][j] = life->dup_map[i][j];
}

static inline bool	read_input(t_life *life)
{
	
	char c;
	while (true)
	{
		ssize_t	bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read == -1)
			return false;
		if (bytes_read == 0)
			return true;
		switch (c)
		{
			case 'x':
				life->can_write = !life->can_write;
			break;
			case 'w': // up
				life->pen_x--;
			break;
			case 'a': // left
				life->pen_y--;
			break;
			case 's': // down
				life->pen_x++;
			break;
			case 'd': // right
				life->pen_y++;
			break;
		}
		if (life->can_write && is_inside_limits(life->pen_x, life->pen_y, life))
			life->map[life->pen_x][life->pen_y] = life->alive;
	}
}

static inline void	play(t_life *life)
{
	while (life->iterations--)
		conway(life);
}

static inline void	draw_map(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->witdh; j++)
			putchar(life->map[i][j]);
		putchar('\n');
	}
	// putchar('\n');
}

#endif // LIFE_H