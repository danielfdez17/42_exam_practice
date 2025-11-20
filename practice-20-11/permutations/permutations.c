#include <stdio.h>
#include <stdbool.h>

int	ft_strlen(char *str)
{
	int i = -1;
	while (str && str[++i]);
	return i;
}

void	print_sol(char *str, int len)
{
	(void)len;
	printf("%s\n", str);
}

void	ft_swap(char *a, char *b)
{
	char c = *a;
	*a = *b;
	*b = c;
}

void	ft_bubblesort(char *str, int len)
{
	for (int i = 0; i < len; ++i)
		for (int j = i; j < len; ++j)
			if (str[i] > str[j])
				ft_swap(&str[i], &str[j]);
}

bool	find_next_permutation(char *str, int len)
{
	int i = len - 2;
	while (i >= 0 && str[i] >= str[i + 1])
		--i;
	if (i < 0)
		return (false);
	int j = len - 1;
	while (j >= 0 && str[j] <= str[i])
		--j;
	ft_swap(&str[j], &str[i]);
	int start = i + 1;
	int end = len - 1;
	while (start < end)
		ft_swap(&str[start++], &str[end--]);
	return true;
}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	char *str = av[1];
	int len = ft_strlen(av[1]);
	ft_bubblesort(str, len);
	print_sol(str, len);
	while (find_next_permutation(str, len))
		print_sol(str, len);
	return 0;
}