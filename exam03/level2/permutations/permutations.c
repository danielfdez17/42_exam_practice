#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void	ft_swap(char *a, char *b)
{
	char tmp = *a;
	*a = *b;
	*b = tmp;
}

void	ft_putstr(char *str)
{
	int i = -1;
	while (str[++i])
		write(STDOUT_FILENO, &str[i], 1);
	write(STDOUT_FILENO, "\n", 1);
}

int	ft_strlen(char *str)
{
	int i = -1;
	while (str[++i]);
	return (i);
}

void	ft_sort(char *str, int len)
{
	int i = -1;
	while (++i < len)
	{
		int j = -1;
		while (++j < len - i - 1)
		{
			if (str[j] > str[j + 1])
				ft_swap(&str[j], &str[j + 1]);
		}
	}
}

bool	ft_next_permutation(char *str, int len)
{
	int i = len - 2;
	while (i >= 0 && str[i] >= str[i + 1])
		--i;
	if (i < 0)
		return (false);
	int j = len - 1;
	while (str[j] <= str[i])
		--j;
	ft_swap(&str[j], &str[i]);
	int start = i + 1;
	int end = j - 1;
	while (start < end)
		ft_swap(&str[start++], &str[end--]);
	return (true);
}

int main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	char *str = av[1];
	int len = strlen(str);
	ft_sort(str, len);
	ft_putstr(str);
	while (ft_next_permutation(str, len))
		ft_putstr(str);
	return (0);
}
