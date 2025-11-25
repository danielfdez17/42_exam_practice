#include <stdio.h>
#include <unistd.h>

int	ft_count(char *str)
{
	int i = -1;
	int open = 0, close = 0;
	while (str[++i])
	{
		if (str[i] == '(')
			++open;
		else if (str[i] == ')')
		{
			if (open > 0)
				--open;
			else
				close++;
		}
	}
	return (open + close);
}

void	solve(char *str, int i, int open, int close, int erased, int count, char *buffer)
{
	if (!str[i])
	{
		//! no dispairs
		if (open == close && erased == count)
		{
			buffer[i] = '\0';
			puts(buffer);
		}
		return ;
	}
	char c = str[i];
	// ! select str[i]
	if (c == '(' || c == ')')
	{
		buffer[i] = c;
		if (c == '(')
			solve(str, i + 1, open + 1, close, erased, count, buffer);
		else if (open > close)
			solve(str, i + 1, open, close + 1, erased, count, buffer);
	}
	else
	{
		buffer[i] = c;
		solve(str, i + 1, open, close, erased, count, buffer);
	}
	// ! discard str[i]
	if (c == '(' || c == ')')
	{
		buffer[i] = ' ';
		solve(str, i + 1, open, close, erased + 1, count, buffer);
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	char *str = av[1];
	char buffer[1024];
	int count = ft_count(str);
	solve(str, 0, 0, 0, 0, count, buffer);
	return 0;
}
