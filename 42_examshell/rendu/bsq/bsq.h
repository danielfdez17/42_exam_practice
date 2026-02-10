#ifndef BSQ_H
#define BSQ_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

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

static inline bool	is_printable(char c)
{
	return c >= 32 && c <= 126;
}

static inline bool	is_inside_limits(int x, int y, int height, int width)
{
	return x >= 0 && x < height && y >= 0 && y < width;
}

static inline bool	read_info(FILE *file, t_info *info)
{
	int	ret = fscanf(file, "%d %c %c %c", &info->height, &info->empty, &info->obstacle, &info->full);
	if (ret != 4)
		return false;
	if (info->height <= 0)
		return false;
	info->width = -1;
	info->biggest_sq.x = 0;
	info->biggest_sq.y = 0;
	info->biggest_sq.size = 0;
	return is_printable(info->full) && is_printable(info->obstacle) && is_printable(info->full);
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

static inline bool	read_file(FILE *file, t_info *info)
{
	char	*line = NULL;
	size_t	bytes_read = 0;
	if (getline(&line, &bytes_read, file) <= 0)
		return false;
	
	for (int i = 0; i < info->height; i++)
	{
		int read = getline(&line, &bytes_read, file);
		if (read <= 0)
		{
			free(line);
			return false;
		}
		if (info->width == -1)
			info->width = read;
		else if (info->width != read)
		{
			free(line);
			return false;
		}
		clear_new_lines(&line);

		for (int j = 0; j < info->width; j++)
		{
			map[i][j] = line[j];
		}
		
	}

	free(line);
	return true;
	
}

static inline bool is_valid_map(t_info *info, t_square sq)
{
	if (sq.size <= 0)
		return false;
	if (!is_inside_limits(sq.x + sq.size, sq.y + sq.size, info->height, info->width))
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
					if (sq.size > info->biggest_sq.size)
					{
						info->biggest_sq = sq;
					}
				}
				
			}
		}
	}
}

static inline void	fill_map(t_info *info)
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

static inline void	print_map(t_info *info)
{
	for (int i = 0; i < info->height; i++)
		fprintf(stdout, "%s\n", map[i]);
}

static inline bool	bsq(FILE *file)
{
	t_info info;
	// read_info
	if (!read_info(file, &info))
		return false;
	// read_file
	if (!read_file(file, &info))
		return false;
	// search_biggest
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