#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

void	ft_putstr(char *str, int len)
{
	for (int i = 0; i < len; ++i)
		write(STDOUT_FILENO, &str[i], 1);
	write(STDOUT_FILENO, "\n", 1);
}

void	ft_swap(char *a, char *b)
{
	char c = *a;
	*a = *b;
	*b = c;
}

int	ft_strlen(char *str)
{
	int i = 0;
	while (str && str[i])
		++i;
	return i;
}

bool	find_next_permutation(char *str, int len)
{
	// ! find flag from behind
	int i = len - 2;
	while (i >= 0 && str[i] >= str[i + 1])
		--i;
	if (i < 0)
		return false;
	// ! find next flag from behind
	int j = len - 1;
	while (j >= 0 && str[i] >= str[j])
		--j;
	// ! swap them
	ft_swap(&str[i], &str[j]);
	// ! reverse from first_flag + 1
	int start = i + 1;
	int end = len - 1;
	while (start < end)
		ft_swap(&str[start++], &str[end--]);
	return true;
}

void	ft_bubblesort(char *str, int len)
{
	for (int i = 0; i < len; ++i)
		for (int j = i; j < len; ++j)
			if (str[i] > str[j])
				ft_swap(&str[i], &str[j]);
}


int main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	char *str = av[1];
	int len = ft_strlen(str);
	ft_bubblesort(str, len);
	ft_putstr(str, len);
	while (find_next_permutation(str, len))
		ft_putstr(str, len);
	return 0;
}