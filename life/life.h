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
}	t_life;

static inline bool	free_map(t_life *life)
{
	int i = -1;
	while (++i < life->height)
		free(life->map[i]);
	free(life->map);
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
	
	for (int i = 0; i < life->witdh; i++)
	{
		life->map[i] = (char *)calloc(sizeof(char), life->witdh);
		if (!life->map[i])
			return free_map(life);
	}

	life->alive = 'O';
	life->dead = ' ';
	life->pen_x = 0;
	life->pen_y = 0;
	
	return true;
}

static inline bool	is_inside_limits(int x, int y, t_life *life)
{
	return x >= 0 && x < life->height && y >= 0 && y < life->witdh;
}

static inline bool	conway(t_life *life)
{
	printf("Unimplemented method\n");
	// dup map
	// free dup map
}

static inline bool	read_input(t_life *life)
{
	
	printf("Unimplemented method\n");
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
			break;
			case 'a': // left
			break;
			case 's': // down
			break;
			case 'd': // right
			break;
		}
	}
}

static inline void	play(t_life *life)
{

	while (life->iterations--)
		conway(life);
}

static inline void	draw_map(t_life *life)
{
	printf("Unimplemented method\n");
}

#endif // LIFE_H