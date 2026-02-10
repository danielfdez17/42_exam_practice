#include "life.h"

int main(int ac, char **av)
{
	if (ac != 4)
		return 1;
	t_life life;
	if (!init_life(&life, av))
		return 1;
	read_movs(&life);
	conway(&life);
	print_map(&life);
	free_life(&life);
}
