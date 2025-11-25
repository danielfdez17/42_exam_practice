#include "get_next_line.h"

static char *ft_clean_buffer(char *str) // ! advace one str pos
{
	int i = 0;
	int str_len = ft_strlen(str);
	while (str && str[i] && str[i] != '\n')
		++i;
	if (!str[i])
		return (free(str), NULL);
	char *new_buff = ft_substr(str, i + 1, str_len);
	free(str);
	return (new_buff);
}

static char *ft_till_endl(char *str)
{
	int i = 0;
	while (str && str[i])
	{
		if (str[i] == '\n')
			return (ft_substr(str, 0, i + 1));
		++i;
	}
	return (ft_substr(str, 0, i));
}

static char	*ft_read_file(int fd, char *static_buff)
{
	int bytes = 1;
	char *buffer;
	long	buffer_size = BUFFER_SIZE;
	while (!ft_find_endl(static_buff) && bytes > 0)
	{
		buffer = malloc(sizeof(char) * (buffer_size + 1));
		if (!buffer)
			return (NULL);
		bytes = read(fd, buffer, buffer_size);
		if (bytes <= 0)
		{
			free(buffer);
			return (static_buff);
		}
		buffer[bytes] = '\0';
		static_buff = ft_strjoin(static_buff, buffer);
		buffer_size *= 2;
	}
	return (static_buff);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char	*line;

	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = ft_read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_till_endl(buffer);
	buffer = ft_clean_buffer(buffer); // ! free param
	return (line);
}

//? clear && cc -Wall -Wextra -Werror -fsanitize=address -g get_next_line.c get_next_line_utils.c

int main(void)
{
	char *line;
	// int fd = open("test", O_RDONLY);
	// if (fd < 0)
	// 	return (1);
	while ((line = get_next_line(0)))
	{
		printf("%s", line);
		free(line);
	}
	// close(fd);
	return (0);
}