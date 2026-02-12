#ifndef BSQ_H
#define BSQ_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200809L

#define MAX_SIZE 1024

char	map[MAX_SIZE][MAX_SIZE] = {0};

typedef struct s_square
{
	int			x;
	int			y;
	int			size;
} t_square;

typedef struct s_info
{
	int			width;
	int			height;
	char		empty;
	char		obstacle;
	char		full;
	t_square	biggest;
} t_info;

static inline bool is_printable(char c)
{
	return c >= 32 && c <= 126;
}

static inline bool is_inside_limits(int x, int y, int height, int width)
{
	return x >= 0 && x < height && y >= 0 && y < width;
}

static inline bool	read_info(FILE *file, t_info *info)
{
	int ret = fscanf(file, "%d %c %c %c", &info->height, &info->empty, &info->obstacle, &info->full);
	if (ret != 4)
		return false;
	if (info->height <= 0)
		return false;
	if (info->empty == info->obstacle || info->empty == info->full || info->obstacle == info->full)
		return false;
	info->biggest.x = 0;
	info->biggest.y = 0;
	info->biggest.size = 0;
	info->width = -1;
	return is_printable(info->obstacle) && is_printable(info->full) && is_printable(info->empty);
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

static inline bool	is_valid_map(t_info *info, t_square sq)
{
	if (sq.size == 0)
		return false;
	
	if (!is_inside_limits(sq.x + sq.size, sq.y + sq.size, info->height, info->width))
		return false;
	
	for (int i = sq.x; i <= sq.size + sq.x; i++)
	{
		for (int j = sq.y; j <= sq.size + sq.y; j++)
			if (map[i][j] == info->obstacle)
				return false;
	}
	return true;
}

// static inline bool has_valid_chars(t_info *info, char *line)
// {
// 	size_t i = 0;
// 	while (line[i])
// 	{
// 		if (line[i] != info->obstacle && line[i] != info->full && line[i] != info->obstacle)
// 			return false;
// 		i++;
// 	}
// 	return true;
// }

static inline void	fill_map(t_info *info)
{
	for (int i = info->biggest.x; i <= info->biggest.size + info->biggest.x; i++)
	{
		for (int j = info->biggest.y; j <= info->biggest.size + info->biggest.y; j++)
			map[i][j] = info->full;
	}
}

static inline void	search_biggest(t_info *info)
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
				if (sq.size > info->biggest.size && is_valid_map(info, sq))
				{
					info->biggest = sq;
				}
			}
		}
	}
}

static inline bool	read_map(FILE *file, t_info *info)
{
	char *line = NULL;
	size_t read_bytes = 0;
	if (getline(&line, &read_bytes, file) <= 0)
		return false;

	for (int i = 0; i < info->height; i++)
	{
		int read = getline(&line, &read_bytes, file);
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
		// if (!has_valid_chars(info, line))
		// {
		// 	free(line);
		// 	return false;
		// }
		for (int j = 0; j < info->width; j++)
			map[i][j] = line[j];
	}
	
	free(line);
	return true;
}

static inline void	print_map(t_info *info)
{
	for (int i = 0; i < info->height; i++)
		fprintf(stdout, "%s\n", map[i]);
}

static inline bool bsq(FILE *file)
{
	t_info info;
	// read_info
	if (!read_info(file, &info))
		return false;
	// fprintf(stdout, "read_info ok\n");
	// read_map
	if (!read_map(file, &info))
		return false;
	// fprintf(stdout, "read_file ok\n");
	// search_biggest
	search_biggest(&info);
	// fill_map
	fill_map(&info);
	// print_map
	print_map(&info);
	return true;
}

static inline bool convert_to_ptr(char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
		return false;
	bool ret = bsq(file);
	fclose(file);
	return ret;
}

#endif // BSQ_H