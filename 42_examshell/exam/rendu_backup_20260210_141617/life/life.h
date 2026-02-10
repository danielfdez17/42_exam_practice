#ifndef LIFE_H
#define LIFE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct	s_life
{
	int		width;
	int		height;
	int		iterations;
	char	**map;
	char	**dup_map;
	int		pen_x;
	int		pen_y;
	bool	can_write;
	char	dead;
	char	alive;
}	t_life;

static inline bool	is_inside_limits(int x, int y, int height, int width)
{
	return x >= 0 && x < height && y >= 0 && y < width;
}

static inline bool is_alive(int x, int y, t_life *life)
{
	
}

static inline bool free_life(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		if (life->map[i])
			free(life->map[i]);
	}
	if (life->map)
		free(life->map);
	for (int i = 0; i < life->height; i++)
	{
		if (life->dup_map[i])
			free(life->dup_map[i]);
	}
	if (life->dup_map)
		free(life->dup_map);
	return false;
}
// ! init both maps
static inline bool init_life(t_life *life, char **av)
{
	life->width = atoi(av[1]);
	if (life->width <= 0)
		return false;
	life->height = atoi(av[2]);
	if (life->width <= 0)
		return false;
	life->iterations = atoi(av[3]);
	if (life->iterations < 0)
		return false;

	life->map = calloc(sizeof(char **), life->height);
	if (!life->map)
		return false;
	
	for (int i = 0; i < life->width; i++)
	{
		life->map[i] = calloc(sizeof(char *), life->width);
		if (!life->map[i])
			return free_life(life);
	}

	life->dup_map = calloc(sizeof(char **), life->height);
	if (!life->dup_map)
		return free_life(life);
	
	for (int i = 0; i < life->width; i++)
	{
		life->dup_map[i] = calloc(sizeof(char *), life->width);
		if (!life->dup_map[i])
			return free_life(life);
	}
	life->pen_x = 0;
	life->pen_y = 0;
	life->can_write = false;
	life->dead = ' ';
	life->alive = 'O';
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

static inline void	read_info(t_life *life)
{
	char c ;
	while (true)
	{
		ssize_t bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0)
			return ;
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
		if (is_inside_limits(life->pen_x, life->pen_y, life->height, life->width))
		{
			if (life->can_write)
				life->map[life->pen_x][life->pen_y] = life->alive;
		}
		// else
		// 	life->map[life->pen_x][life->pen_y] = life->dead;
	}
}

static inline void print_map(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			putchar(life->map[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
}

static inline int count_neighbours(int x, int y, t_life *life)
{
	int n = 0;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (is_inside_limits(i, j, life->height, life->width))
		}
	}
}

static inline void conway(t_life *life)
{
	
}

static inline void play(t_life *life)
{
	while (life->iterations--)
		conway(life);
}

#endif // LIFE_H