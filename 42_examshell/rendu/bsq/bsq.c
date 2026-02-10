#include "bsq.h"

int main(int ac, char **av)
{
	if (ac == 1)
	{
		bsq(stdin);
		return 0;
	}
	for (int i = 1; i < ac; i++)
	{
		if (convert_to_ptr(av[i]))
		{
			if (i + 1 < ac)
			{
				fprintf(stdout, "\n");
			}
		}
	}
	return 0;
}
