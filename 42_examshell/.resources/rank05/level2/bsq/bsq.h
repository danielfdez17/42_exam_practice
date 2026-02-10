#ifndef BSQ_H
#define BSQ_H

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_SIZE 1024

char map[MAX_SIZE][MAX_SIZE] = {0};

typedef struct s_square
{
	int		x;
	int		y;
	int		size;
}	t_square;

typedef struct s_info
{
	int			height;
	int			width;
	char		empty;
	char		obstacle;
	char		full;
	t_square	biggest_sq;
}	t_info;

static inline bool is_inside_limits(int x, int y, int height, int width)
{
	return x >= 0 && x < height && y >= 0 && y < width;
}

static inline bool read_info(FILE *file, t_info *info)
{
	info->width = -1; // to check lines' sizes
	info->biggest_sq.x = 0;
	info->biggest_sq.y = 0;
	info->biggest_sq.size = 0;
	int ret = fscanf(file, "%d %c %c %c", &info->height, &info->empty, &info->obstacle, &info->full);
	if (ret != 4)
		return false;
	
	if (info->height <= 0)
		return false;
	
	if (info->empty == info->obstacle || info->empty == info->full || info->obstacle == info->full)
		return false;
	return true;
}

static inline void clear_new_lines(char **line)
{
	char *ptr = *line;
	size_t i = 0;
	while (ptr[i])
	{
		if (ptr[i] == '\n')
			ptr[i] = '\0';
		i++;
	}
}

static inline bool read_map(FILE *file, t_info *info)
{
	char *line = NULL;
	size_t	read_bytes = 0;
	if (getline(&line, &read_bytes, file) <= 0)
		return false;
	
	for (int i = 0; i < info->height; i++)
	{
		int read = getline(&line, &read_bytes, file);
		if (read <= 0) // ? EOF
		{
			free(line);
			return false;
		}
		if (info->width == -1)
			info->width = read;
		else if (info->width != read) // ? different lines' sizes
		{
			free(line);
			return false;
		}
		clear_new_lines(&line);
		// fprintf(stdout, "width: %d\n", info->width);

		// ? copy line to map
		for (int j = 0; j < info->width; j++)
			map[i][j] = line[j];
		
	}

	free(line);
	return true;
}

static inline void	print_map(t_info *info)
{
	for (int i = 0; i < info->height; i++)
	{
		fprintf(stdout, "%s\n", map[i]);
	}
	// fprintf(stdout, "\n");
}

static inline bool is_valid_map(t_info *info, t_square sq)
{
	if (sq.size <= 0
		|| !is_inside_limits(sq.x + sq.size, sq.y + sq.size, info->height, info->width))
		return false;
	for (int i = sq.x; i <= sq.x + sq.size; i++)
	{
		for (int j = sq.y; j <= sq.y + sq.size; j++)
		{
			if (map[i][j] == info->obstacle)
				return false;
		}
	}
	return true;
}

static inline void fill_map(t_info *info)
{
	for (int i = info->biggest_sq.x; i <= info->biggest_sq.x + info->biggest_sq.size; i++)
	{
		for (int j = info->biggest_sq.y; j <= info->biggest_sq.y + info->biggest_sq.size; j++)
		{
			if (map[i][j] == info->obstacle)
				return ;
			map[i][j] = info->full;
		}
	}
}

// static inline void fill_map(t_info *info)
// {
// 	for (int i = info->biggest_sq.x; i < info->biggest_sq.x + info->biggest_sq.size; i++)
// 	{
// 		for (int j = 0; j < info->biggest_sq.y + info->biggest_sq.size; j++)
// 		{
// 			map[i][j] = info->full;
// 		}
// 	}
// }

static inline void	find_biggest(t_info *info)
{
	t_square sq;
	for (int i = 0; i < info->height; i++)
	{
		sq.x = i;
		for (int j = 0; j < info->width; j++)
		{
			sq.y = j;
			for (int k = 0; k < info->width && k < info->height; k++)
			{
				sq.size = k;
				if (is_valid_map(info, sq))
				{
					if (sq.size > info->biggest_sq.size
						&& is_inside_limits(sq.x + sq.size, sq.y + sq.size, info->height, info->width))
					{
						// fprintf(stdout, "(%d,%d)\n", sq.x + sq.size, sq.y + sq.size);
						info->biggest_sq = sq;
						// fill_map(info);
						// print_map(info);
						// reset_map(info);
					}
				}
			}
		}
	}
}

static inline bool	bsq(FILE *file)
{
	t_info info;
	// read_info
	if (!read_info(file, &info))
		return false;
	// read_map
	if (!read_map(file, &info))
		return false;
	// search_biggest_sq
	find_biggest(&info);
	// fill_map
	fill_map(&info);
	// print_map
	print_map(&info);
	return true;
}

static inline bool	convert_to_ptr(char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
		return false;
	bool ret = bsq(file);
	fclose(file);
	return ret;
}

#endif // BSQ_H