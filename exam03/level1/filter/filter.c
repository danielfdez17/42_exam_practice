
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif

void	ft_filter(char *buffer, char *str)
{
	int len = strlen(str);
	int i = 0;
	while (buffer[i])
	{
		int j = 0;
		while (str[j] && buffer[i+j] == str[j])
			++j;
		if (j == len) // ! str inside buffer
		{
			int aux = -1;
			while (++aux < len)
				write(STDOUT_FILENO, "*", 1);
			i += j;
		}
		else
		{
			write(STDOUT_FILENO, &buffer[i], 1);
			++i;
		}
	}
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;
	char buffer[999999];
	int bytes_read = 1;
	int i = 0;
	if (ac != 2 || !av[1][0])
		return (1);
	while (bytes_read > 0)
	{
		bytes_read = read(STDIN_FILENO, &buffer[i], BUFFER_SIZE);
		if (bytes_read < 0)
		{
			perror("Error");
			return (1);
		}
		i += bytes_read;
	}
	ft_filter(buffer, av[1]);
}
// ! cc -Wall -Wextra -Werror filter.c -o filter