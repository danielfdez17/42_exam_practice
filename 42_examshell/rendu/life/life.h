#ifndef LIFE_H
#define LIFE_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct s_life
{
	int 	width;
	int 	height;
	int 	iterations;
	int 	pen_x;
	int 	pen_y;
	bool	can_write;
	char	**map;
	char	**aux_map;
	char	alive;
	char	dead;
}	t_life;

static inline bool is_inside_limits(int x, int y, int height, int width)
{
	return x >= 0 && x < height && y >= 0 && y < width;
}

static inline bool	is_alive(int x, int y, t_life *life)
{
	if (!is_inside_limits(x, y, life->height, life->width))
		return false;
	return life->map[x][y] == life->alive;
}

static inline bool	is_dead(int x, int y, t_life *life)
{
	if (!is_inside_limits(x, y, life->height, life->width))
		return false;
	return life->map[x][y] == life->dead;
}

static inline bool	free_life(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		if (life->map[i])
			free(life->map[i]);
	}
	free(life->map);
	for (int i = 0; i < life->height; i++)
	{
		if (life->aux_map[i])
			free(life->aux_map[i]);
	}
	free(life->aux_map);
	return false;
}

static inline bool init_life(t_life *life, char **av)
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
	life->can_write = false;

	life->map = (char**)calloc(sizeof(char *), life->height);
	if (!life->map)
		return false;
	
	for (int i = 0; i < life->height; i++)
	{
		life->map[i] = (char *)calloc(sizeof(char), life->width);
		if (!life->map[i])
			return free_life(life);
	}

	life->aux_map = (char**)calloc(sizeof(char *), life->height);
	if (!life->aux_map)
		return free_life(life);
	
	for (int i = 0; i < life->height; i++)
	{
		life->aux_map[i] = (char *)calloc(sizeof(char), life->width);
		if (!life->aux_map[i])
			return free_life(life);
	}

	life->alive = 'O';
	life->dead = ' ';

	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			life->map[i][j] = life->dead;
			life->aux_map[i][j] = life->dead;
		}
	}
	
	return true;
}

static inline void read_movs(t_life *life)
{
	while (true)
	{
		char c;
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
		}

		if (is_inside_limits(life->pen_x, life->pen_y, life->height, life->width))
		{
			if (life->can_write)
				life->map[life->pen_x][life->pen_y] = life->alive;
		}
		
	}
	
}

static inline void update_map(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			life->map[i][j] = life->aux_map[i][j];
		}
	}
}

static inline int count_neighbours(t_life *life, int x, int y)
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

static inline void play(t_life *life)
{
	for (int i = 0; i < life->height; i++)
	{
		for (int j = 0; j < life->width; j++)
		{
			int n = count_neighbours(life, i, j);
			if (is_alive(i, j, life))
			{
				if (n < 2 || n > 3)
					life->aux_map[i][j] = life->dead;
				else
					life->aux_map[i][j] = life->map[i][j];
			}
			else if (is_dead(i, j, life))
			{
				if (n == 3)
					life->aux_map[i][j] = life->alive;
				else
					life->aux_map[i][j] = life->map[i][j];
			}
		}
	}
	update_map(life);
}

static inline void conway(t_life *life)
{
	while (life->iterations--)
		play(life);
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
	// putchar('\n');
}

#endif // LIFE_H