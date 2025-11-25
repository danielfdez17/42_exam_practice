#include <unistd.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif

int	ft_strlen(char *str)
{
	int i = 0;
	while (str && str[i])
		++i;
	return (i);
}

void	ft_filter(char *buffer, char *str)
{
	int str_len = ft_strlen(str);
	int i = 0;
	while (buffer[i])
	{
		int j = 0;
		while (str[j] == buffer[i + j])
			++j;
		if (j == str_len)
		{
			i += j;
			while (j--)
				printf("*");
		}
		else
			printf("%c", buffer[i++]);
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	char buffer[999999];
	int bytes = 1;
	int i = 0;
	while (bytes > 0)
	{
		bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE);
		i += bytes;
	}
	ft_filter(buffer, av[1]);
	return 0;
}
