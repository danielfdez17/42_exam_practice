#include "bsq.h"

int main(int ac, char **av)
{
	if (ac == 1)
	{
		if (!bsq(stdin))
			return 1;
		return 0;
	}
	for (int i = 1; i < ac; i++)
	{
		if (convert_to_ptr(av[1]))
		{
			if (i + 1 < ac)
				fprintf(stdout, "\n");
		}
	}
	return 0;
}
