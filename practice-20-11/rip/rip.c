#include <stdio.h>

int	ft_count(char *str)
{
	int open = 0, close = 0;
	int i = -1;
	while (str[++i])
	{
		if (str[i] == '(')
			++open;
		else if (str[i] == ')')
		{
			if (open > 0)
				--open;
			else
				++close;
		}
	}
	return (open + close);
}

void	print_solutions(char *str, int i, int open, int close, int removed, int count, char *buffer)
{
	if (str[i] == '\0')
	{
		if (open == close && removed == count)
		{
			buffer[i] = '\0';
			puts(buffer);
		}
		return ;
	}
	char c = str[i];
	//! Aceptarlo
	if (c == '(' || c == ')')
	{
		buffer[i] = c;
		if (c == '(')
			print_solutions(str, i + 1, open + 1, close, removed, count, buffer);
		else if (open > close)
			print_solutions(str, i + 1, open, close + 1, removed, count, buffer);
	}
	else
	{
		buffer[i] = c;
		print_solutions(str, i + 1, open, close, removed, count, buffer);
	}
	//! Rechazarlo
	if (c == '(' || c == ')')
	{
		buffer[i] = ' ';
		print_solutions(str, i + 1, open, close, removed + 1, count, buffer);
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	char *str = av[1];
	int count = ft_count(str);
	char buffer[1024];
	print_solutions(str, 0, 0, 0, 0, count, buffer);
	return 0;
}